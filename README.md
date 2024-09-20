# Control Proporcional de Temperatura e Intensidad de Luz
<h2>Descripción</h2>
<p>
  Desarrollo de un control proporcional de temperatura utilizando el
conversor análogo-digital y el módulo CCP del microcontrolador PIC16F877.
</p>
<h2>Características</h2>
<p>
  Se utiliza al menos un sensor de temperatura que genere una señal analógica de voltaje.
</p>
<p>
  Se utiliza un actuador para subir la temperatura y otro actuador para
disminuir la temperatura, ambos controlados por señales PWM generadas
por los módulos CCP.
</p>
<p>
  Hay un setpoint por defecto, y permite la opción de cambiar el
setpoint a través del teclado matricial. El setpoint se almacena en
la EEPROM.
</p>
<p>
  En la pantalla LCD se muestra el valor actual de temperatura, el
setpoint y el ciclo de trabajo de cada actuador.
</p>
