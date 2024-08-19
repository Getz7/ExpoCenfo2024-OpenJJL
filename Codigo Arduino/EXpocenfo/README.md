# Proyecto: Monitoreo de Frecuencia Cardíaca con Ideaboard y Envío de Datos a un Servidor
Este proyecto utiliza un sensor de pulso, un lector RFID, LEDs, una pantalla LCD y un módulo WiFi para medir la frecuencia cardíaca de una persona y enviar los datos a un servidor web. La pantalla LCD muestra información relevante durante el proceso, y los LEDs indican diferentes estados en función de la respuesta del servidor.

## Descripción del Hardware Utilizado

- **WiFi**: Se utiliza el módulo WiFi para conectar el dispositivo a internet y enviar los datos al servidor.
- **HTTPClient**: Se usa para realizar solicitudes HTTP al servidor.
- **PulseSensorPlayground**: Este sensor mide la frecuencia cardíaca.
- **FastLED**: Controla los LEDs, los cuales muestran diferentes colores según el estado de la operación.
- **LCD-I2C**: Pantalla LCD utilizada para mostrar mensajes al usuario.
- **EasyMFRC522**: Lector RFID que se utiliza para identificar la tarjeta antes de comenzar a medir la frecuencia cardíaca.
## Configuración de Hardware

### Pines Utilizados:
- **Pin de pulso**: `33`
- **Pin de datos del LED**: `2`
- **Pines del lector RFID**: `4 (SDA)` y `5 (SCK)`

### Credenciales de WiFi:
- **SSID**: ` depende de conexión`
- **Contraseña**: `depende de conexión`

### URL del Servidor:
- **serverUrl**: `https://cardiology-assistant-v2.vercel.app/api/assistant-pulse-rate`

## Explicación del Código

### `setup()`
Este método se ejecuta una vez al inicio. Aquí se inicializan todos los componentes de hardware, como la pantalla LCD, el lector RFID, el sensor de pulso, y los LEDs. También se establece la conexión WiFi.

- **Inicialización del LCD**: Se enciende la pantalla y se configura la retroiluminación.
- **Inicialización del lector RFID**: Se configura el lector RFID para que esté listo para detectar tarjetas.
- **Inicialización de LEDs**: Configura los LEDs para apagarse al inicio.
- **Conexión a WiFi**: Llama a la función `connectToWiFi()` para conectar el dispositivo a una red WiFi.
- **Configuración del sensor de pulso**: Establece el pin de entrada y el umbral de detección para el sensor de pulso.

### `loop()`
Este método se ejecuta en bucle después de la configuración inicial, realizando las siguientes tareas:

1. **Esperar tarjeta RFID**: Llama al método `waitForRFID()` para esperar que una tarjeta sea detectada antes de proceder.
2. **Medir la frecuencia cardíaca**: Una vez detectada la tarjeta, solicita al usuario que coloque su dedo en el sensor de pulso y mide la frecuencia cardíaca.
3. **Enviar datos al servidor**: Crea un objeto JSON con la frecuencia cardíaca medida y lo envía al servidor a través de una solicitud HTTP POST.
4. **Indicar estado con LEDs**: Dependiendo de la respuesta del servidor, muestra diferentes mensajes en la pantalla LCD y cambia los colores de los LEDs.
5. **Esperar**: Espera un tiempo antes de repetir el proceso completo.

### `connectToWiFi()`
Este método conecta el dispositivo a una red WiFi utilizando las credenciales especificadas.

- **Visualización en pantalla**: Muestra el estado de la conexión en la pantalla LCD.
- **Reintento de conexión**: Si la conexión falla, sigue intentando conectarse hasta que tenga éxito.

### `waitForRFID()`
Este método espera hasta que se detecte una tarjeta RFID antes de proceder. Es útil para asegurar que el usuario está autorizado antes de medir su frecuencia cardíaca.

- **Detectar tarjeta**: El método `detectTag()` del objeto `rfidReader` se utiliza para comprobar si hay una tarjeta presente.
- **Indicaciones en pantalla**: Muestra mensajes en la pantalla LCD durante el proceso.

### `mainProcess()`
Este método principal dentro del bucle `loop()` realiza las siguientes tareas:

1. **Medir la frecuencia cardíaca**: Llama al método `getBeatsPerMinute()` del objeto `pulseSensor` para obtener la frecuencia cardíaca.
2. **Crear objeto JSON**: Crea un objeto JSON con la frecuencia cardíaca medida para enviarlo al servidor.
3. **Enviar solicitud HTTP**: Realiza una solicitud HTTP POST al servidor con el objeto JSON.
4. **Manejo de respuestas**: Maneja las respuestas del servidor y actualiza la pantalla LCD y los LEDs en consecuencia.

## Estado de los LEDs

- **Azul**: El servidor ha respondido con un código 200 (ya tienes planes).
- **Rojo**: El servidor ha respondido con un código 500 (error en el servidor).
- **Verde**: El servidor ha respondido con un código 202 (todo en orden).
- **Naranja**: Se han creado nuevos planes.
- **Apagado**: Estado por defecto cuando no se realiza ninguna acción.

