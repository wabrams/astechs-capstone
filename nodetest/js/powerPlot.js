var interval = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9];
var powdata  = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0];

var trace1 = {
  x: interval,
  y: powdata,
  type: 'scatter'
};

var data = [trace1];

var layout = {
  title: 'Power Consumption',
  showlegend: false,
  paper_bgcolor: "rgba(0,0,0,0)",
  plot_bgcolor:  "rgba(0,0,0,0)",
  margin: {l: 0, r: 0, b: 0, t: 10, pad: 4},
};

Plotly.newPlot('livePlot1', data, layout);

function updateData(powread)
{
  for (let i = 0; i < 10 - 1; i++)
    powdata[i] = powdata[i + 1];
  powdata[10 - 1] = powread;
}

function updatePlot()
{
  data[0]['y'] = powdata;
  Plotly.redraw('livePlot1');
}

console.log(powdata);
updateData(1);
updateData(2);
updateData(3);
console.log(powdata);