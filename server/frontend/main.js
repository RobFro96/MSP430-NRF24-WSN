const graphs = [
    {
        "element": "weatherStationTemperature",
        "ylabel": "temperature (°C)",
        "series": [
            { "sensor": 1, "column": "si7021_temperature", "color": "tab:red" }
        ]
    },
    {
        "element": "weatherStationHumidity",
        "ylabel": "relative humidity (%)",
        "series": [
            { "sensor": 1, "column": "si7021_humidity", "color": "tab:blue" }
        ]
    },
    {
        "element": "weatherStationPressure",
        "ylabel": "air pressure (hPa)",
        "series": [
            { "sensor": 1, "column": "bmp180_pressure", "color": "tab:green" }
        ]
    },
    {
        "element": "homeTemperature",
        "ylabel": "temperature (°C)",
        "series": [
            { "sensor": 2, "column": "temperature_internal", "legend": "Bad Fenster" },
            { "sensor": 3, "column": "temperature_internal", "legend": "Bad Heizung" }
        ]
    },
    {
        "element": "homeReed",
        "series": [
            { "sensor": 2, "column": "reed", "legend": "Bad Fenster" }
        ]
    },
    {
        "element": "wsnRetries",
        "ylabel": "retries",
        "series": [
            { "sensor": 1, "column": "retries", "legend": "Wetterstation" },
            { "sensor": 2, "column": "retries", "legend": "Bad Fenster" },
            { "sensor": 3, "column": "retries", "legend": "Bad Heizung" }
        ]
    },
]

$(function () {
    setDatePicker(new Date());
    show();

    $("#dateShowButton").click(show);
    $("#dateLeftButton").click(dateLeftClicked);
    $("#dateRightButton").click(dateRightClicked);
});

function setDatePicker(date) {
    document.getElementById("datePicker").value = date.getFullYear() + '-' + ('0' + (date.getMonth() + 1)).slice(-2) + '-' + ('0' + date.getDate()).slice(-2);
}

function show() {
    let date = document.getElementById("datePicker").value;
    console.log(date);

    for (let graph of graphs) {
        let json = Object.assign({}, graph);
        json.date = date;
        let url = "graph/" + JSON.stringify(json);
        $("#" + graph.element).attr("src", url);
    }

}

function dateLeftClicked() {
    skipDate(-1);
}

function dateRightClicked() {
    skipDate(1);
}

function skipDate(delta) {
    let date = new Date($("#datePicker").val());
    date.setDate(date.getDate() + delta);
    setDatePicker(date);
    show();
}