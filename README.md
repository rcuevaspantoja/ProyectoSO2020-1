### Hola internet :earth_americas:, me llamo Rodolfo y soy estudiante de último año de Ingeniería Civil en informática de la Universidad del BioBío en Concepción, Chile.





### Redes :arrow_down_small:
[<img align="left" alt="fitocuevas | Twitter" width="22px" src="http://cdn.jsdelivr.net/npm/simple-icons@v3/icons/twitter.svg" />][Twitter] 
[<img align="left" alt="fitocuevas | Instagram" width="22px" src="http://cdn.jsdelivr.net/npm/simple-icons@v3/icons/instagram.svg" />][Instagram]
[<img align="left" alt="fitocuevas | Gmail" width="22px" src="https://cdn.jsdelivr.net/npm/simple-icons@3.5.0/icons/gmail.svg" />][Gmail]
[<img align="left" alt="fitocuevas | Streaming" width="22px" src="https://cdn.jsdelivr.net/npm/simple-icons@3.5.0/icons/twitch.svg" />][TwitchTv]

<br />

# :bookmark_tabs: Introducción.

Para este proyecto semestral se solicitó un programa en C que pueda crear procesos que puedan ejecutar un programa llamado loop.c y que estos procesos puedan ser ejecutando en 
3 posibles algoritmos de planificación donde todos los procesos tengan la oportunidad de ser ejecutados. Aplicando conocimiento de Estructura de datos, creación, permutación y 
sincronización de procesos, y uso de señales.

<br />

# :bookmark_tabs: Desarrollo. 

Para este desarrollo se utilizó un trabajo constante de 8 horas al día durante 5 días. Que consistió en un estudio, planeación e implementación con constantes pruebas de código.
La estructura de datos(EDD) elegida no fue la óptima, ya que esta fue plenamente elegida para el algoritmo de planificación Round Robin, adaptando los otros 2 algoritmos pedidos a 
esta estructura. Primero, se implemento la creación de procesos hijos que ejecutarán un archivo llamada loop.c por medio de un proceso padre para luego irse deteniendo a medida que 
iban siendo creados. Después y mediante 
una cola ciruclar se van almacenando los <pid> (id identificadora de procesos) para ser trasladados a funciones distintas donde se les dará una ejecución diferente dependiendo del
proceso que se haya seleccionado, como se especifica mejor en el informe adjunto.


[Twitter]: https://twitter.com/elfitocuevas
[Instagram]: https://instagram.com/fitocuevas
[Gmail]: mailto:rcuevaspantoja@gmail.com
[TwitchTv]: https://twitch.tv/reformedteemo
