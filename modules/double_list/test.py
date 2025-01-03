import numpy as np
import matplotlib.pyplot as plt

# Statische Variablen
V_L = 100  # Beispielwert
F_G = 10000  # Grenzfrequenz in Hz

# Frequenzbereich (logarithmisch von 10 Hz bis 1 MHz)
f = np.logspace(1, 6, 1000)

# Berechnung der Funktion
y = V_L * 1 / np.sqrt(1 + (f/F_G)**2)

# Plot erstellen
plt.figure(figsize=(10, 6))
plt.loglog(f, y)  # Beide Achsen logarithmisch
plt.grid(True)
plt.xlabel('Frequenz [Hz]')
plt.ylabel('Faktor')
plt.title('Frequenzgang')
plt.axvline(F_G, color='r', linestyle='--', alpha=0.5, label='Grenzfrequenz')
plt.legend()
plt.show()