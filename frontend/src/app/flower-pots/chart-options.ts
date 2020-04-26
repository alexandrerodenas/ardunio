export const chartOptions = (value: number) => {
  return {
    chart: {
      type: 'gauge',
      backgroundColor: 'transparent',
      width: 500
    },
    title: null,
    pane: {
      startAngle: -90,
      endAngle: 90,
      background: null,
      center: ['50%', '50%']
    },
    tooltip: {
      enabled: false
    },
    yAxis: {
      min: 0,
      max: 100,

      minorTickInterval: 'auto',
      minorTickWidth: 1,
      minorTickLength: 10,
      minorTickPosition: 'inside',
      minorTickColor: '#666',

      tickPixelInterval: 30,
      tickWidth: 2,
      tickPosition: 'inside',
      tickLength: 10,
      tickColor: '#666',
      labels: {
        step: 2,
        rotation: 'auto'
      },
      title: {
        text: 'humidity %'
      },
      plotBands: [{
        from: 0,
        to: 33,
        thickness: 30,
        color: '#DF5353' // green #DF5353
      }, {
        from: 33,
        to: 66,
        thickness: 30,
        color: '#DDDF0D' // yellow
      }, {
        from: 66,
        to: 100,
        thickness: 30,
        color: '#55BF3B' // red
      }, {
        from: 90,
        to: 100,
        color: '#6677ff',
        innerRadius: '100%',
        outerRadius: '110%'
      }]
    },
    series: [{
      name: 'Humidity',
      data: [value],
      dataLabels: {
        borderWidth: 0,
        format:
          '<div style="text-align:center; border:none">' +
          '<span style="font-size:25px">{y}%</span><br/>' +
          '</div>'
      },
      tooltip: {
        valueSuffix: ' %'
      }
    }],
    credits: {
      enabled: false
    }
  };
};
