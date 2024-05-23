# Estación Meteorológica con un ESP32

Se muestra en detalle cómo construir una estación meterorológica para medir presión atmosférica, humedad ambiental, temperatura ambiental y concentraciones de algunos gases (PTHG). El sistema usa un microcontrolador ESP32 que se programa mediante el software gratuito Arduino IDE con el código que se muestra aquí. Los valores de PTHG se miden cada 5 segundos y se mandan por WiFi a un servidor en el que se almacenan. El servidor recibe estos datos usando un código que se corre con node.js y que se muestra aquí. También se incluye el código en HTML y JavaScript para desplegar gráficas con estos datos entre dos fechas dadas usando cualquier navegador web.



El sensor que usamos para medir presión, temperatura y humedad es el BME280. Para medir condentración de algunos gases usamos los sensores MQ2 (gas licuado de petroleo, LP, y gas natural vehicular, GNV), MQ6 (propano, butano y metano) y MQ135 (amoniaco, alcohol, benceno, humo y dióxido de carbono). Los valores que se muestran de los sensores de gases no están calibrados. Se pueden fácilmente agregar otros sensores para crecer la capacidad de esta estación con modificaciones ligeras del software.



## Uso

- Seguir las instrucciones que se incluyen en la página wed de demo


## Demo en línea

https://ciiec.buap.mx/Estacion'Meteo/

## Licencia

[MIT](LICENSE)
