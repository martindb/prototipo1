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
- Estado de las camaras (temperatura)
- Si hay energia electrica (x fase)
- Si hay internet


Tecnologias:
- MQTT
  - Payload en json
- Node-RED
- Cloud?

