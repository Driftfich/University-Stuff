import numpy as np
import matplotlib.pyplot as plt

# Statische Variablen
V_L = 100  # Beispielwert
V_L2 = 1
F_G = 10000  # Grenzfrequenz in Hz
F_G2 = 1000000

# Frequenzbereich (logarithmisch von 10 Hz bis 1 MHz)
f = np.logspace(1, 6, 1000)

# Berechnung der Funktion
y = V_L * 1 / np.sqrt(1 + (f/F_G)**2)
y2 = V_L2 * 1 / np.sqrt(1 + (f/F_G2)**2)
# y2 = np.clip(y2, a_min=0, a_max=None)   
x_messung = np.array([30, 1000, 3000, 8000, 10000, 12000, 15000, 20000, 30000, 50000])
y_messung = np.array([100, 100, 94, 72, 63, 56, 47.7, 38, 26, 15.8])
y2_messung = np.array([1]*10)

x_messung_b = np.array([0, 30, 1000, 3000, 8000, 10000, 12000, 15000, 20000, 30000, 50000])
y_messung_b = np.array([100, 100, 100, 94, 72, 63, 56, 47.7, 38, 26, 15.8])

# dB Berechnung
y_db = 20 * np.log10(y)
y_messung_db = 20 * np.log10(y_messung/V_L)

# Plot mit zwei logarithmischen Achsen
fig, ax1 = plt.subplots(figsize=(10, 6))

# Linke Y-Achse (logarithmisch)
ax1.loglog(f, y, label='Frequenzgang a) (berechnet)', color="#00cad2")
ax1.plot(f, y2, label='Frequenzgang b) (berechnet)', color="#ffd25f")
ax1.plot(x_messung, y_messung, 'o-', color='#00bae4', label='Frequenzgang a) (gemessen)')
ax1.plot(x_messung, y2_messung, 'o-', color='#de8d0b', label='Frequenzgang b) (gemessen)')
ax1.grid(True, which="major", linestyle="-", alpha=0.5)
ax1.grid(True, which="minor", linestyle=":", alpha=0.2)
ax1.set_xlabel('Frequenz f [Hz]')
ax1.set_ylabel('Verstärkungsfaktor v(f)')

# # Rechte Y-Achse für dB (ohne Plot)
# ax2 = ax1.twinx()
# ax2.set_yscale('log')
# ax2.set_ylabel('Verstärkung [dB]')

# # Synchronisiere Y-Achsen
# y1_ticks = ax1.get_yticks()
# ax2.set_ylim(ax1.get_ylim())
# ax2.set_yticks(y1_ticks)
# ax2.set_yticklabels([f'{20*np.log10(y):.1f}' for y in y1_ticks])

# Grenzfrequenz
ax1.axvline(F_G, color='#004087', linestyle='--', alpha=0.5, label='Grenzfrequenz a)')
ax1.axvline(F_G2, color='#af2200', linestyle='--', alpha=0.5, label='Grenzfrequenz b)')
ax1.axvline(0.8*F_G, linestyle='--', alpha=0.5, label='Grenzfrequenz a) (gemessen)', color="#af005f")

# mark the area under the curve with a color
ax1.fill_between(x_messung_b[0:5], y_messung_b[0:5], color="#af005f", alpha=0.1, label="Bandbreite B für a) (gemessen)")

# Legende
lines1, labels1 = ax1.get_legend_handles_labels()
ax1.legend(lines1, labels1, loc='upper right')

plt.show()