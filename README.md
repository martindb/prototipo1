## Prototipo supervisor deposito

Repo del prototipo 1 de supervisor de deposito.

Algunas de las ideas:
- ESP8266 (nodemcu)
- Conexion electrica y bateria de respaldo
  - debe durar unas cuantas horas.
  - hay que hacerlo andar en deep sleep (ojo con los accesorios)
  - tener en cuenta el load sharing / powerpath
- GSM/GPRS (impresindible para avisar si se corta luz/internet)
- Sensores
  - Temperatura x N (dallas 18b20)
  - Fases 220v x3 (para saber si se corto una o dos fases)
- Debe ser economico, serviseable a lo largo del tiempo, con cosas que se consigan en MercadoLibre


Que debe informar:
- Estado de la/las camaras (temperatura)
- Si hay energia electrica (x fase, 3)
- Si hay internet
- Info del modulo (power/bat, vbat, ?)


Tecnologias:
- MQTT
  - Payload en json
- Node-RED
- Cloud?


Hardware:
- nodemcu esp8266
- powerbank x4 (https://articulo.mercadolibre.com.ar/MLA-899661886-modulo-cargador-power-bank-x4-bateria-18650-usb-c-micro-3a-_JM)
- sim800 (https://articulo.mercadolibre.com.ar/MLA-876768223-modulo-celular-gsm-gprs-sim800-arduino-m2m-iot-sim800l-_JM)
- 18db20 (tengo algunos, comprar mas)
- optoacopladores (alterna vs mods al modulo?)
- fuente integrada? (da la potencia x1?)

