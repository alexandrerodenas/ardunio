/*
function refresh() {
  setTimeout(refresh, 2000);
  fetchMeasuresFromServer();
}

// initial call, or just call refresh directly
refresh();
*/


function fetchMeasuresFromServer() {
  $.ajax({
    type: "GET",
    url: 'http://192.168.0.17/measures.json',
    cache: false,
    success: function (data) {
      updateCircleValues(data);
    }
  });
}

$('#refresh').on('click', () => {
  fetchMeasuresFromServer();
});

$(document).ready(() => {
  fetchMeasuresFromServer();
})

function updateCircleValues(values) {
  console.log(values);
  myCircle1.update(values[0]);
  myCircle2.update(values[1]);
  myCircle3.update(values[2]);
}

var myCircle1 = Circles.create({
  id: 'circles-1',
  radius: 60,
  value: 43,
  maxValue: 100,
  width: 10,
  text: function (value) {
    return value + '%';
  },
  colors: ['#BEE3F7', '#45AEEA'],
  duration: 400,
  wrpClass: 'circles-wrp',
  textClass: 'circles-text',
  valueStrokeClass: 'circles-valueStroke',
  maxValueStrokeClass: 'circles-maxValueStroke',
  styleWrapper: true,
  styleText: true
});
var myCircle2 = Circles.create({
  id: 'circles-2',
  radius: 60,
  value: 43,
  maxValue: 100,
  width: 10,
  text: function (value) {
    return value + '%';
  },
  colors: ['#BEE3F7', '#45AEEA'],
  duration: 400,
  wrpClass: 'circles-wrp',
  textClass: 'circles-text',
  valueStrokeClass: 'circles-valueStroke',
  maxValueStrokeClass: 'circles-maxValueStroke',
  styleWrapper: true,
  styleText: true
});
var myCircle3 = Circles.create({
  id: 'circles-3',
  radius: 60,
  value: 43,
  maxValue: 100,
  width: 10,
  text: function (value) {
    return value + '%';
  },
  colors: ['#BEE3F7', '#45AEEA'],
  duration: 400,
  wrpClass: 'circles-wrp',
  textClass: 'circles-text',
  valueStrokeClass: 'circles-valueStroke',
  maxValueStrokeClass: 'circles-maxValueStroke',
  styleWrapper: true,
  styleText: true
});

