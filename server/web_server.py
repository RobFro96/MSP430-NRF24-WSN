import datetime
import io
import json
import logging
import sqlite3
import threading
import time
import urllib.parse

import flask
import matplotlib
import matplotlib.pyplot as plt
import werkzeug.serving
from matplotlib.backends.backend_svg import FigureCanvasSVG

from config import config


class WebServer(threading.Thread):
    """Klasse des Webservers
    Methode zum Laufenlassen des Flask-Servers
    """

    def __init__(self, database: sqlite3.Connection):
        threading.Thread.__init__(self)
        self.database = database

        self.app = flask.Flask(__name__, static_url_path="", static_folder="frontend/")  # Flask-App
        self.app.debug = True
        self.server = None  # Werkzeug-Server

        self.app.add_url_rule("/", "index", self.__index)
        self.app.add_url_rule("/graph/<obj>", view_func=self.__graph)

    def __index(self):
        """Endpunkt für index.html
        """
        return flask.send_file("frontend/index.html")

    def run(self):
        """Starten des Servers
        Der Befehlszeiger bleibt in dieser Methode, bis der Server durch die stop-Methode beendet
        wird.
        """
        self.server = werkzeug.serving.make_server(
            host=config.server_host,
            port=config.server_port,
            app=self.app,
            threaded=True
        )
        self.server.serve_forever()

    def stop(self):
        """Methode zum Stoppen des Servers
        self.server.shutdown() muss in einen anderen Thread gestartet werden -> sonst kommt es zu
        einem Deadlock!
        """
        def stop():
            if self.server:
                self.server.shutdown()
        threading.Thread(target=stop).start()

    def __graph(self, obj):
        try:
            obj = json.loads(urllib.parse.unquote(obj))
        except:
            return "bad request! cannot parse JSON", 400

        if not "date" in obj:
            return "bad request! no date specified", 400

        try:
            start = datetime.datetime.strptime(obj["date"], "%Y-%m-%d")
        except:
            return "bad request! cannot parse date", 400

        end = datetime.timedelta(days=1) + start
        start = time.mktime(start.timetuple())
        end = time.mktime(end.timetuple())

        if not "series" in obj:
            return "bad request! no series specified", 400

        fig = plt.figure()
        axis = fig.add_subplot(1, 1, 1)
        enable_legend = False

        for s in obj["series"]:
            if ("sensor" not in s) or ("column" not in s):
                return "bad request! sensor or column not specitfied in %s" % s, 400

            try:
                data = self.database.execute(
                    """SELECT time, %(column)s
                    FROM Data
                    WHERE time >= %(start)d AND time < %(end)d AND addr=%(sensor)s""" % {
                        "column": s["column"],
                        "start": start,
                        "end": end,
                        "sensor": s["sensor"]
                    }).fetchall()
            except:
                logging.exception("")
                return "bad request! Error reading database!"

            timestamps = [datetime.datetime.fromtimestamp(t) for t, v in data]
            values = [v for t, v in data]

            axis.plot(timestamps, values, color=s.get("color"), label=s.get("legend"))
            if s.get("legend"):
                enable_legend = True

        axis.xaxis.set_major_formatter(matplotlib.dates.DateFormatter('%H:%M'))
        if enable_legend:
            axis.legend()
        axis.set_ylabel(obj.get("ylabel"))
        axis.grid(True)

        output = io.BytesIO()
        # FigureCanvasSVG(fig).print_svg(output)
        fig.savefig(output, format="svg", bbox_inches="tight")
        return flask.Response(output.getvalue(), mimetype="image/svg+xml")
