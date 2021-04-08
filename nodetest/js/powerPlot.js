var interval = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9];
var powdata1  = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
var powdata2  = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0];

var trace1 = {
  x: interval,
  y: powdata1,
  type: 'scatter'
};

var trace2 = {
  x: interval,
  y: powdata1,
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

function updateData1(powread)
{
  for (let i = 0; i < 10 - 1; i++)
    powdata1[i] = powdata1[i + 1];
  powdata1[10 - 1] = powread;
}

function updateData2(powread)
{
  for (let i = 0; i < 10 - 1; i++)
    powdata2[i] = powdata2[i + 1];
  powdata2[10 - 1] = powread;
}

function updatePlot()
{
  data[0]['y'] = powdata1;
  data[0]['y'] = powdata2;
  Plotly.redraw('livePlot1');
}