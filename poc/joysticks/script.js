var touches = [];
var mainTouchArea = document.getElementById('main-touch-area');

function updateEventStore(eventArray, status) {
  for(var i = 0; i < eventArray.length; i++) {
    var t = eventArray[i];
    if (t.target.id === 'main-touch-area') { return; }
    touchStore(t.target.id, status, t.pageX, t.pageY)
  }
}

mainTouchArea.addEventListener('touchstart', function(event) {
  updateEventStore(event.targetTouches, 'START')
}, false);

mainTouchArea.addEventListener('touchmove', function(event) {
  event.preventDefault();
  updateEventStore(event.targetTouches, 'MOVE')
}, false);

mainTouchArea.addEventListener('touchend', function(event) {
  updateEventStore(event.changedTouches, 'END')
}, false);

timer = setInterval(function() {
  renderTouches();
}, 15);

function renderTouches() {
  document.getElementById('right').innerHTML = '';
  document.getElementById('left').innerHTML = '';

  for(var i = 0; i < touches.length; i++) {
    document.getElementById(touches[i].identifier).innerHTML =
    '<p>' + touches[i].identifier +
      '<hr>' + (touches[i].startY - touches[i].currentY) + '%</p>';
  }
}

function touchStore(identifier, status, x, y) {
  var touchIndex = touches.findIndex(function(t) {
    return t.identifier === identifier;
  });

  function addTouch() {
    if (touchIndex > -1) { return; }

    touches.push({
      identifier: identifier,
      startX: x,
      startY: y,
      currentX: x,
      currentY: y,
    });
  }

  function deleteTouch() {
    touches.splice(touchIndex, 1);
  }

  function updateTouch() {
    if (touchIndex === -1) { return; }

    touches[touchIndex].currentX = x;
    touches[touchIndex].currentY = y;
  }

  switch(true) {
    case status === 'START':
      return addTouch();
    case status === 'END':
      return deleteTouch();
    case status === 'MOVE':
      return updateTouch();
  }
}
