$(function () {
    var data = [
        {
            x: ['2013-10-04 22:23:00', '2013-10-04 22:24:00', '2013-10-04 22:25:00'],
            y: [1, 3, 6],
            type: 'scatter'
        },
        {
            x: ['2013-10-04 22:23:00', '2013-10-04 22:24:00', '2013-10-04 22:25:00'],
            y: [2, 6, 9],
            type: 'scatter'
        }
    ];

    Plotly.newPlot("temperaturePlot", data, { margin: { t: 10 } }, { displayModeBar: false });
});