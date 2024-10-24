
<p align="center">
    <h1>Proyecto de Estructuras Abstractas de Datos y Algoritmos para Ingeniería - IE0217</h1>
    <h3>Investigación, diseño e implementación de un sistema de gestión bancaria.</h3>
    <p>Profesor: Esteban Badilla Alvarado</p>
    <p>Estudiantes: <br>Sergio Castillo Víquez, C31824 <br>
    Rodrigo Sánchez Araya, C37259<br>
    Jonathan Monge Cascante, 
    </p>
</p>


## Primera Fase del Proyecto - Parte Teórica 

## Definición de términos e ideas
Para tener una idea clara de lo que se quiere realizar se deben saber los términos utilizados dentro de este proyecto, por lo tanto, se debe investigar y saber que son cada uno de ellos y saber que conllevan y como funcionan, con el fin de que a la hora de implementarlos dentro del código ya sea como una función o de alguna otra forma, estos sean realistas y se apeguen a un funcionamiento real.

### Atención a clientes
Basándonos en un banco podemos observar las opciones de atención a clientes que este brinda, por ejemplo, el Scotiabank da un número telefónico al que se puede llamar también permite un chat en línea, por lo tanto, se pude implementar que esta opción brindé estas opciones. Sin embargo, 

### Préstamos Personales
Están enfocados en cubrir necesidades a corto plazo y en general enfocados en gastos propios o familiares, por lo general y basándonos en el banco Scotiabank (Banco de referencia) las tasas de interés van de 23% a 24% en colones y son de tasa fija también estos prestamos en este banco son realizables únicamente en la opción de colones, sin embargo, por indicación el monto del préstamo será ingresado por el usuario, quien también podrá personalizar los valores de monto, número de cuotas y tasa de interés.

### Préstamos Prendarios
Estos préstamos son principalmente financiamientos para compra de bienes de capital, vehículos principalmente, por lo que se toma como un sistema de financiamiento que permite obtener la aprobación del crédito para vehículo, nuevamente tomando como base el banco Scotiabank este da primas desde el 10%, plazos de hasta 96 meses y la opción de colones y dólares, pero nuevamente el monto del préstamo será ingresado por el usuario, también podrá personalizar los valores de monto, número de cuotas y tasa de interés.

### Préstamos Hipotecarios
Los prestamos hipotecarios son prestamos a mediano o largo plazo y por lo general se otorga para la compra, ampliación, reparación o construcción de una vivienda, compra de sitios, oficinas o locales comerciales y son generalmente créditos que permiten a las personas adquirir una vivienda. Estos basándonos en el banco ya dicho ofrece este crédito en la opción de dólares o colones con tasas de 10 a 10,50% en colones y de 6,95 a 8,50% en dólares. Nuevamente y al igual que los anteriores prestamos dichos el monto del préstamo será ingresado por el usuario, también podrá personalizar los valores de monto, número de cuotas y tasa de interés.

### Abonos a préstamos propios o de terceros
Los abonos son pagos en una cantidad adicional al pago mínimo requerido por el préstamo con la cual se tiene una obligación, y esa cantidad extra se destina directamente a reducir el monto principal del préstamo. Por lo que esta se ve como una reducción directa al monto total del préstamo que puede ser tanto a el personal como al de un tercero.

### Tasa de interés, plazo en meses y cuota mensual en los préstamos
El interés es un precio que el cliente paga a la entidad por disponer del capital prestado. El Plazo es un periodo de tiempo estipulado en el contrato para devolver el principal más el interés. Las cuotas son la cantidad que se deben pagar cada mes e incluyen el interés y la parte proporcional de la cantidad utilizada del total del préstamo.

### Abonos al capital en préstamos
Es abonar al total del prestamo y no a los intereses, entonces, como cada mes la cuota llega por un monto de capital y un monto de intereses, en algunos créditos se puede abonar a capital, de modo que se reduzca la deuda y solo quede debiendo los intereses.

### Intereses abonados en préstamos
Los abonos a interés son pagos adicionales que se realizan sobre la deuda, más allá del pago mínimo mensual requerido.

### Fórmulas para el cálculo de los interés y cuotas mensuales de los prestamos
Para la obtención de estas formulas nos basamos en los cálculos que se utilizan en la siguiente página: https://www.billin.net/simulador-prestamos-personales-calculadora/

$\text{Cuota Mensual} = \frac{\text{Capital} \times \text{Interés Mensual}}{1 - (1 + \text{Interés Mensual})^{-12}}$

$\text{Cuota de Interés Mensual} = \frac{\text{Cuota Restante} \times \text{Interés Mensual}}{12}$

### Certificados de depósito a plazo (CDP)
Es un depósito que se pacta a un periodo de tiempo, el cual es acordado entre el cliente y el banco. La tasa de interés se establece en base al monto del depósito y el plazo. Es una alternativa de inversión que hace crecer moderadamente el dinero de la cuenta. Básicamente es una herramienta de ahorros de bajo riesgo que puede aumentar el monto que gana por interés, y al mismo tiempo mantener su dinero invertido de una manera relativamente segura.

### Depósitos
Son un producto de ahorro a través del cual los clientes depositan una cantidad de dinero en la entidad financiera durante un periodo de tiempo determinado. Puede verse como un aumento en el saldo disponible de la cuenta bancaria. 

### Retiro 
Un retiro bancario consiste en extraer dinero de un banco, por ende, se refiere a una reducción de dinero de la cuenta bancaria. 

### Transferencia 
Es una operación por la que una persona desde una cuenta suya envíe una determinada cantidad de dinero a la cuenta de otra persona que puede ser de su mismo banco o de otro. Este puede verse como un movimiento en la cuenta del usuario.
