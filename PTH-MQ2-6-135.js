const express = require('express');
const fs = require('fs');
const app = express();
const PORT = 3000;
var moment = require('moment');
var time = moment();
var time_format = time.format('YYYY-MM-DDTHH:mm:ss');

app.get('/enviar_TPAH', (req, res) => {
    const valor1 = req.query.valor1;
    const valor2 = req.query.valor2;
    const valor3 = req.query.valor3;
    const valor4 = req.query.valor4;
    const valor5 = req.query.valor5;
    const valor6 = req.query.valor6;

    if (!valor1 || !valor2 || !valor3 || !valor4 || !valor5 || !valor6) {
        res.status(400).json({ error: 'Se requieren todos los valores en la consulta.' });
    } else {
        const fechaYTiempo = obtenerFechaYTiempoLocal()
        const timestamp = obtenerTimestampLocal();
        var currentTime = new Date()/1000 -6*3600;
        var now = new Date();
        const valores = `${currentTime},${valor1},${valor2},${valor3},${valor4},${valor5},${valor6}\n`;

        // Agregar los valores al final del archivo (puedes cambiar el nombre del archivo según tus necesidades)
        fs.appendFile('regPTH-MQ2-6-135.csv', valores, 'utf8', (err) => {
            if (err) {
                console.error(err);
                res.status(500).json({ error: 'Error al escribir en el archivo.' });
            } else {
                console.log('Valores agregados al archivo:', valores);
                res.json({ mensaje: 'Valores agregados correctamente al archivo.' });
            }
        });
    }
});



function obtenerFechaYTiempoLocal() {
  const fecha = new Date();

  // Obtiene los componentes de la fecha y tiempo
  const year = fecha.getFullYear().toString().slice(-2); // Obtiene los últimos dos dígitos del año
  const month = agregarCeroAlInicio(fecha.getMonth() + 1); // El mes es indexado desde 0
  const day = agregarCeroAlInicio(fecha.getDate());
  const hours = agregarCeroAlInicio(fecha.getHours());
  const minutes = agregarCeroAlInicio(fecha.getMinutes());
    const seconds = agregarCeroAlInicio(fecha.getSeconds());


  // Formatea la fecha y tiempo como YYmmDDHHMM
  const formato = `${year}${month}${day}${hours}${minutes}${seconds}`;

  return formato;
}

function agregarCeroAlInicio(valor) {
  // Agrega un cero al inicio si el valor es menor que 10
  return valor < 10 ? `0${valor}` : valor;
}

function obtenerTimestampLocal() {
    const ahora = new Date();
    const offset = ahora.getTimezoneOffset();
    const offsetMillis = offset * 60 * 1000;
    const timestampLocal = new Date(ahora.getTime() - offsetMillis).toLocaleString('es-ES', { timeZone: 'UTC' });
    return timestampLocal.replace(/,/g, ''); // Eliminar la coma presente en algunos formatos locales
}

app.listen(PORT, () => {
    console.log(`Servidor escuchando en el puerto ${PORT}`);
});
