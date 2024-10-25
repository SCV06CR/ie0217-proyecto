
<p align="center">
    <h1>Proyecto de Estructuras Abstractas de Datos y Algoritmos para Ingeniería - IE0217</h1>
    <h3>Investigación, diseño e implementación de un sistema de gestión bancaria.</h3>
    <p>Profesor: Esteban Badilla Alvarado</p>
    <p>Estudiantes: <br>Sergio Castillo Víquez, C31824 <br>
    Rodrigo Sánchez Araya, C37259<br>
    Jonathan Monge Cascante, B94955
    </p>
</p>

## Tabla de contenidos
- [Primera Fase del Proyecto - Parte Teórica](#primera-fase-del-proyecto-parte-teórica)
- [Definición de términos e ideas](#definición-de-términos-e-ideas)
- [Primera Fase del Proyecto - Parte de Diseño](#primera-fase-del-proyecto-parte-de-diseño)
- [Cronograma de actividades](#cronograma-de-actividades)
- [Referencias](#referencias)

## Primera Fase del Proyecto - Parte Teórica

## Definición de términos e ideas
Para tener una idea clara de lo que se quiere realizar es necesario definir los términos utilizados dentro de este proyecto, por lo tanto, se debe investigar y definir cada uno de ellos y determinar qué conllevan y cómo se implementan. Esto con el fin de que, a la hora de implementarlos dentro del código, ya sea como una función o de alguna otra forma, estos sean realistas y se apeguen a un funcionamiento real.

### Atención a clientes
Basándose en un banco, podemos observar las opciones de atención a clientes que este brinda. Por ejemplo, el Scotiabank proporciona un número telefónico al que se puede llamar, también permite un chat en línea, por lo tanto, se puede implementar que esta opción brinde la información de contacto necesaria. 

### Préstamos Personales
Están enfocados en cubrir necesidades a corto plazo y en general enfocados en gastos propios o familiares, por lo general y basándose en el banco Scotiabank las tasas de interés van de 23% a 24% en colones y son de tasa fija. Además, estos préstamos en este banco son realizables únicamente en la opción de colones, estos datos también son muy distintos dependiendo del banco en el que se busque, sin embargo, por indicación el monto del préstamo será ingresado por el usuario, quien también podrá personalizar los valores de monto, número de cuotas y la tasa de interés.

### Préstamos Prendarios
Estos préstamos son principalmente financiamientos para compra de bienes de capital, vehículos principalmente, por lo que se toma como un sistema de financiamiento que permite obtener la aprobación del crédito para vehículo, nuevamente tomando como base el banco Scotiabank este, por ejemplo, da primas desde el 10%, plazos de hasta 96 meses y la opción de colones y dólares, pero nuevamente el monto del préstamo será ingresado por el usuario, también podrá personalizar los valores de monto, número de cuotas y tasa de interés.

### Préstamos Hipotecarios
Los préstamos hipotecarios son préstamos a mediano o largo plazo y, por lo general, se otorgan para la compra, ampliación, reparación o construcción de una vivienda, compra de sitios, oficinas o locales comerciales y son generalmente créditos que permiten a las personas adquirir una vivienda. Estos, basándonos en el banco ya dicho ofrece este crédito en la opción de dólares o colones con tasas de 10 a 10,50% en colones y de 6,95 a 8,50% en dólares. Nuevamente y al igual que los anteriores préstamos dichos, el monto del préstamo será ingresado por el usuario; también podrá personalizar los valores de monto, número de cuotas y tasa de interés.

### Abonos a préstamos propios o de terceros
Los abonos son pagos en una cantidad adicional al pago mínimo requerido por el préstamo con la cual se tiene una obligación, y esa cantidad extra se destina directamente a reducir el monto principal del préstamo. Por lo que esta se ve como una reducción directa al monto total del préstamo que puede ser tanto a el personal como al de un tercero.

### Tasa de interés, plazo en meses y cuota mensual en los préstamos
El interés es un precio que el cliente paga a la entidad por disponer del capital prestado. El Plazo es un periodo de tiempo estipulado en el contrato para devolver el principal más el interés. Las cuotas son la cantidad que se deben pagar cada mes e incluyen el interés y la parte proporcional de la cantidad utilizada del total del préstamo.

### Tipos de interés
**Interés fijo :** 
El interés fijo refiere al tipo de interés que se mantiene constante a lo largo del tiempo del préstamo, es decir, se mantiene invariable respecto a la tasa de interés del mercado.

**Interés variable :** 
Al contrario del interés fijo, este es actualizado respecto a la tasa de interés del mercado bajo plazos establecidos por la entidad financiera. Además, la entidad define previamente un rango dentro del cual varía el tipo de interés.

**Interés mixto :**
Cómo lo indica su nombre, este tipo de interés mezcla los dos tipos de interés anteriores, de modo que durante los primeros años del préstamo el tipo de interés es fijo; sin embargo, durante los años restantes del préstamo, el interés es de tipo variable.

### Abonos al capital en préstamos
Es abonar al total del prestamo y no a los intereses, entonces, como cada mes la cuota llega por un monto de capital y un monto de intereses, en algunos créditos se puede abonar a capital, de modo que se reduzca la deuda y solo quede debiendo los intereses.

### Intereses abonados en préstamos
Los abonos a interés son pagos adicionales que se realizan sobre la deuda, más allá del pago mínimo mensual requerido.

### Fórmulas para el cálculo de los intereses y cuotas mensuales de los prestamos
Para la obtención de estas formulas nos basamos en los cálculos que se utilizan en Simulador de Préstamos Personales y Empresas - Calculadora Billin, 2018 y la pagina oficial del banco BAC Credomatic la cual incluye múltiples documentos con información detallada al respecto. Estas formulas utilizan principalmente los datos de los intereses, el capital o monto total del préstamo pedido y el plazo en meses del préstamo.

### Fórmula para el cálculo de los intereses de los prestamos
$\text{Cuota de Interés Mensual} = \frac{\text{Cuota Restante} \times \text{Interés Mensual}}{12}$

### Fórmula para el cálculo de las cuotas mensuales para los prestamos personales y prendarios
$\text{Cuota Mensual} = \frac{\text{Capital} \times \text{Interés Mensual}}{1 - (1 + \text{Interés Mensual})^{-12}}$

### Fórmula para el cálculo de las cuotas mensuales para los prestamos hipotecarios
$\text{Cuota Mensual} = \frac{P \cdot i}{1 - (1 + i)^{-n}}$


### Certificados de depósito a plazo (CDP)
Es un depósito que se pacta a un periodo de tiempo, el cual es acordado entre el cliente y el banco. La tasa de interés se establece en base al monto del depósito y el plazo. Es una alternativa de inversión que hace crecer moderadamente el dinero de la cuenta. Básicamente es una herramienta de ahorros de bajo riesgo que puede aumentar el monto que gana por interés, y al mismo tiempo mantener su dinero invertido de una manera relativamente segura.

### Depósitos
Son un producto de ahorro a través del cual los clientes depositan una cantidad de dinero en la entidad financiera durante un periodo de tiempo determinado. Puede verse como un aumento en el saldo disponible de la cuenta bancaria. 

### Retiro 
Un retiro bancario consiste en extraer dinero de un banco, por ende, se refiere a una reducción de dinero de la cuenta bancaria de un usuario registrado, siempre y cuando este tenga fondos disponibles. 

### Transferencia 
Es una operación por la que una persona desde una cuenta suya envíe una determinada cantidad de dinero a la cuenta de otra persona que puede ser de su mismo banco o de otro. Este puede verse como un movimiento en la cuenta del usuario.

## Primera Fase del Proyecto: Parte de Diseño 
Concluida la sección teórica, se detalla el diseño que el programa de gestión bancaria necesita para su correcto funcionamiento según las intrucciones asignadas.

## Cronograma de actividades

### Referencias 

1. Billin. (2018). Simulador de préstamos personales y empresas - Calculadora Billin. https://www.billin.net/simulador-prestamos-personales-calculadora/

2. GCFGlobal. Crédito: ¿Cuáles son los tipos de interés? Recuperado el 22 de Octubre, 2024, de https://edu.gcfglobal.org/es/credito/cuales-son-las-tasas-de-interes/1/