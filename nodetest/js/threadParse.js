function toggleNodeImage(divOn, divOff)
{
  var temp = document.getElementById(divOff).style.display;
  document.getElementById(divOff).style.display = document.getElementById(divOn).style.display;
  document.getElementById(divOn ).style.display = temp;
}
function parseCommand(data)
{
  if (data.startsWith('RCS1'))
  {
    if (data.includes('NCS1'))
    {
      toggleNodeImage('n1img_on','n1img_off');
    }
    else if (data.includes('NTB1'))
    {
      toggleNodeImage('n2img_on','n2img_off');
    }
    else if (data.includes('NTB2'))
    {
      toggleNodeImage('n3img_on','n3img_off');
    }
  }
  else if (data.startsWith('BRDA'))
  {
    if (data.includes('POWER'))
    {
      var pread = Number(data.substr(data.indexOf('POWER')+6));
      updateData(pread);
      updatePlot();
    }
  }
}