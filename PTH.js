var ms_start;
var ms_final;



const today = new Date();
const yyyy = today.getFullYear();
let mm = today.getMonth() + 1; // Months start at 0!
let dd = today.getDate();

if (dd < 10) dd = '0' + dd;
if (mm < 10) mm = '0' + mm;

const formattedToday = yyyy + '-' + mm + '-' + dd;

document.getElementById('start_date').value = formattedToday;
document.getElementById('final_date').value = formattedToday;


var toggle_instruc = document.getElementById('toggleInstruc');
toggle_instruc.addEventListener('click', instruc_out);

      
window.onorientationchange = function() {
        var orientation = window.orientation;
            switch(orientation) {
                case 0:
                case 90:
                case -90: window.location.reload();
                break; }
    };
start_date();
final_date();
instruc_out();

function start_date() {
  let x = document.getElementById("start_date").value;
 ms_start = new Date(x)/1000;; 
console.log("Initial " + x + " " + ms_start);
getData();
}

function final_date() {
        let x = document.getElementById("final_date").value;
 ms_final = new Date(x)/1000 + 24*3600;
console.log("Final " + x + " " + ms_final);
getData();
}


getData();
function getData() {
  const csvUrl = "https://ciiec.buap.mx/Estacion-Meteo/regPTH.csv";
        fetch(csvUrl)
          .then(response => {
            if (!response.ok) {
              throw new Error(`HTTP error! Status: ${response.status}`);
            }
            return response.text();
          })
          .then(csvData => {
            const parsedData = parseCSV(csvData);

            if (parsedData.length > 0) {
              // Log the first 10 rows to the console
              //console.log(parsedData.slice(0, 10));

              // Assuming the CSV has 7 columns
              const xValues = Array.from({ length: parsedData.length }, (_, index) => index + 1);
              const yValues = parsedData[0].map((_, colIndex) => parsedData.map(row => row[colIndex]));

              plotData(xValues, yValues);
            } else {
              console.error('Parsed data is empty.');
            }
          })
          .catch(error => console.error('Error fetching or parsing CSV:', error));
     }

    function parseCSV(csvData) {

      var interval = parseInt(document.getElementById('interval').value);
      const rows = csvData.split('\n');
      const data = [];
console.log("Number of rows in file: "+rows.length);
console.log("Row 0 " + rows[0]);
console.log("Row last-1 " + rows[rows.length-2]);
      for (let i = 0; i < rows.length; i=i+interval) {
        const columns = rows[i].split(',').map(value => parseFloat(value.trim()));
        if (columns[0] >= ms_start && columns[0] <= ms_final) {data.push(columns)};
      }

      return data;
    }

    function plotData(xValues, yValues) {
      //console.log(yValues[4])
           // console.log(yValues[5])
            //      console.log(yValues[6])
const my_dates = yValues[0].map(secs => new Date(secs * 1000));
const arrOfDates = my_dates.map(convertirAFecha) ;

//const arrOfDates = yValues[0].map(secs => new Date(secs * 1000).toUTCString());

var data1 = [{
          x: arrOfDates,
          y: yValues[2],
          mode: "lines+markers"
      }];

       var layout1 = {
          xaxis: {
              //   range: [0, N],
              title: "Time",
              type: 'category'
          },
          yaxis: {
              //    range: [-1, 1],
              title: "Pressure (hPa)"
          },
          title: ""
      };


      var data2 = [{
          x: arrOfDates,
          y: yValues[1],
          mode: "lines+markers"
      }];

       var layout2 = {
          xaxis: {
              //   range: [0, N],
              title: "Time",
              type: 'category'
          },
          yaxis: {
              //    range: [-1, 1],
              title: "Temperature (C)"
          },
          title: ""
      };

      var data3 = [{
          x: arrOfDates,
          y: yValues[3],
          mode: "lines+markers"
      }];

       var layout3 = {
          xaxis: {
              //   range: [0, N],
              title: "Time",
              type: 'category'
          },
          yaxis: {
              //    range: [-1, 1],
              title: "Relative Humidity (%)"
          },
          title: ""
      };

var trace41 = {
          x: arrOfDates,
          y: yValues[4],
          name: 'MQ2',
          mode: "lines+markers"
      };
var trace42 = {
          x: arrOfDates,
          y: yValues[5],
          name: 'MQ6',
          mode: "lines+markers"
      };
var trace43 = {
          x: arrOfDates,
          y: yValues[6],
          name: 'MQ135',
          mode: "lines+markers"
      };
var data4 = [trace41,trace42,trace43]
var layout4 = {
          xaxis: {
              //   range: [0, N],
              title: "Time",
              type: 'category'
          },
          yaxis: {
              //    range: [-1, 1],
              title: "ADC Counts"
          },
          title: ""
      };


      Plotly.newPlot('plot1', data1, layout1);
       Plotly.newPlot('plot2', data2, layout2);
        Plotly.newPlot('plot3', data3, layout3);
        Plotly.newPlot('plot4', data4, layout4);
    }

// Función para convertir milisegundos a formato de fecha
 function convertirAFecha(fechaEnMilisegundos) {
     var fecha = new Date(fechaEnMilisegundos);
     var opciones = {
         year: 'numeric',
         month: '2-digit',
         day: '2-digit',
         hour: '2-digit',
         minute: '2-digit',
         second: '2-digit',
         timeZone: 'UTC'  // Configurar la zona horaria a UTC
     };
     return fecha.toLocaleString('es-ES', opciones);
 }

function instruc_out(){
var codeOut = document.getElementById("instruc");
if (codeOut.style.display === "none") {
    codeOut.style.display = "block";
  } else {
    codeOut.style.display = "none";
  }
}