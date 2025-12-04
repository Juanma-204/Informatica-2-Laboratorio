import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# === CONFIGURACIÓN DEL ÁREA (las paredes del simulador) ===
WIDTH = 800   # igual que Walls::width
HEIGHT = 600  # igual que Walls::height

# === CARGAR DATOS DEL CSV ===
df = pd.read_csv("datos.csv")

# Obtener cuántas partículas hay
num_particulas = df["id"].max() + 1
tiempos = df["tiempo"].unique()

# Crear figura
fig, ax = plt.subplots()
ax.set_xlim(0, WIDTH)
ax.set_ylim(0, HEIGHT)

# Dibujar paredes (líneas negras)
ax.plot([0, WIDTH], [0, 0], 'k-')           # piso
ax.plot([0, WIDTH], [HEIGHT, HEIGHT], 'k-') # techo
ax.plot([0, 0], [0, HEIGHT], 'k-')          # izquierda
ax.plot([WIDTH, WIDTH], [0, HEIGHT], 'k-')  # derecha

# Puntos para cada partícula
puntos = [ax.plot([], [], 'o')[0] for _ in range(num_particulas)]

def animar(frame):
    tiempo_actual = frame
    datos_tiempo = df[df["tiempo"] == tiempo_actual]

    for i in range(num_particulas):
        fila = datos_tiempo[datos_tiempo["id"] == i]

        if not fila.empty:
            x = float(fila["x"].iloc[0])
            y = float(fila["y"].iloc[0])

            # Matplotlib exige secuencias (listas)
            puntos[i].set_data([x], [y])

    return puntos

ani = animation.FuncAnimation(
    fig,
    animar,
    frames=len(tiempos),
    interval=50
)

plt.title("Simulación de partículas con paredes")
plt.xlabel("X")
plt.ylabel("Y")

plt.show()
