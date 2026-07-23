
#ifndef _HTML_h
#define _HTML_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

const char html_output_styles[] PROGMEM = R"====(
.led {display: inline-block; width: 10px; height: 10px; border-radius: 50%; margin-right: 5px; }
.led.off {background-color: grey;}
.led.on {background-color: green;}
.led.delayedoff {background-color: orange;}
.btn-group { display: flex; gap: 5px; flex-wrap: nowrap; }
.output-btn { min-width: 70px; padding: 5px 15px; border-radius: 5px; border: 1px solid #888; background: #eee; cursor: pointer; }
.output-btn.on.active { background: #4caf50; color: #fff; }
.output-btn.off.active { background: #f44336; color: #fff; }
.output-btn.auto.active { background: #03a9f4; color: #fff; }
.brightness-input { width: 50px; margin-left: 10px; text-align: center; }
)====";

const char html_form_end[] PROGMEM = R"=====(
</br><form action='/reboot' method='get'><button type='submit'>Reboot</button></form>
</br><form action='/' method='get' style='display:inline;'>
  <button type='submit'>Home</button>
  </br>
</form>
</br><form action='/post' method='post' style='display:inline;' onsubmit="event.preventDefault(); postReset();">
  <button type='submit'>Reset to factory defaults</button>
</form>
<script>
function postReset() {
    fetch('/post', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: 'reset=true'
    })
    .then(response => {
        if (response.ok) { window.location.href = '/'; }
    })
    .catch(error => {
        console.error('Reset fehlgeschlagen:', error);
    });
}
</script>
)=====";

const char html_end_template[] PROGMEM = R"=====(
</div></body></html>
)=====";

const char html_script[] PROGMEM = R"=====(
// Initialize time synchronization
let timeOffset = 0;
initializeTime();

function initializeTime() {
   var xhttp = new XMLHttpRequest();
   xhttp.onreadystatechange = function() {
       if (this.readyState == 4 && this.status == 200) {
           // Parse "YYYY-MM-DD HH:MM:SS" format
           let serverTimeStr = this.responseText.trim();
           let serverTime = new Date(serverTimeStr.replace(' ', 'T'));
           let clientTime = new Date();
           timeOffset = serverTime.getTime() - clientTime.getTime();
           updateDateTime();
           setInterval(updateDateTime, 1000);
       }
   };
   xhttp.open('GET', 'DateTime', true);
   xhttp.send(); 
}

function updateDateTime() {
    let now = new Date(Date.now() + timeOffset);
    // Format as HH:MM:SS
    let timeStr = now.toLocaleTimeString('de-DE', {
        hour: '2-digit',
        minute: '2-digit',
        second: '2-digit',
        hour12: false
    });
    document.getElementById('DateTimeValue').innerHTML = timeStr;
}

// Re-sync every 10 minutes to prevent drift
setInterval(initializeTime, 600000);

// Initial data request and periodic updates every 10 seconds
requestData();
setInterval(requestData, 10000);

function requestData() {
   fetch('/data')
   .then(response => response.json())
   .then(data => updateData(data))
   .catch(error => console.error('Error fetching data:', error));
}

function updateData(jsonData) {
    document.getElementById('RSSIValue').innerHTML = jsonData.RSSI;

    // Update valve status display
    if (jsonData.valve) {
        for (let i = 1; i <= 8; i++) {
            let valveStatus = jsonData.valve['zone' + i];
            let statusSpan = document.getElementById('Zone' + i + 'Status');
            
            if (statusSpan && valveStatus) {
                statusSpan.textContent = valveStatus;
            }
        }
    }

    // Update button states
    if (jsonData.button) {
        for (let i = 1; i <= 8; i++) {
            let buttonState = jsonData.button['zone' + i];
            if (buttonState) {
                setOutputButtonState(String(i), buttonState);
            }
        }
    }

    // Update moisture sensors
    if (jsonData.moisture) {
        for (let i = 1; i <= 2; i++) {
            let moistureValue = jsonData.moisture['sensor' + i];
            let moistureSpan = document.getElementById('SoilMoisture' + i + 'Percent');
            
            if (moistureSpan && moistureValue !== null) {
                moistureSpan.textContent = moistureValue + '%';
            }
        }
    }
}

function setOutputButtonState(ch, state) {
  let group = document.getElementById(ch + 'Btns');
  if (group) {
    group.querySelectorAll('.output-btn').forEach(btn => {
      if (btn.getAttribute('data-state') === state) {
        btn.classList.add('active');
      } else {
        btn.classList.remove('active');
      }
    });
  }
}

document.addEventListener('DOMContentLoaded', function() {
  document.querySelectorAll('.btn-group').forEach(group => {
    group.querySelectorAll('.output-btn').forEach(btn => {
      btn.addEventListener('click', function() {
        // Remove active from all buttons in this group
        group.querySelectorAll('.output-btn').forEach(b => b.classList.remove('active'));
        // Set active on clicked button
        this.classList.add('active');
        // Send state to backend via POST to /post
        let ch = this.getAttribute('data-ch');
        let state = this.getAttribute('data-state');
        let param = '';
        if (ch === '1') param = 'ch1=' + state;
        else if (ch === '2') param = 'ch2=' + state;
        else if (ch === '3') param = 'ch3=' + state;
        else if (ch === '4') param = 'ch4=' + state;
        else if (ch === '5') param = 'ch5=' + state;
        else if (ch === '6') param = 'ch6=' + state;
        else if (ch === '7') param = 'ch7=' + state;
        else if (ch === '8') param = 'ch8=' + state;
        fetch('/post', {
          method: 'POST',
          headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
          body: param
        }).then(response => {
          if (response.ok) {
            requestData();
          }
        }).catch(error => {
          console.error('Error posting data:', error);
        });
      });
    });
  });
});
)=====";

#endif