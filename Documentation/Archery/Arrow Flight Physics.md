Every arrow has a number of physics parameters that define it's behavior:

* **Mass** ($m$) - mass of the arrow;
* **Air Friction** ($a$) - how much air resistance affects the arrow *(0 - 1 range)*;
* **Lift** ($l$) - defines the lift force that is applied to the arrow when it is traveling at the Reference Speed ($v = 10000$ units / second).
* **Stability** ($s$) - determines how much resistance the arrow has to air turbulence (0 - 1 range): 
	* 0 - no resistance; 
	* 1 - complete resistance.

 $\vec{V}$ - arrow's velocity vector, $\vec{L}$ - arrow's current location.

### Gravity
$$
\vec{F_g} = \vec{g} \cdot Mass,
$$
where $g = 981$ --- free fall acceleration in $cm/s^2$.

### Air Friction
$$
\vec{F_a} = -1 \cdot \frac{\vec{V}} {|\vec{V}|} \cdot 0.5 \cdot r \cdot |\vec{V}|^2 \cdot a,
$$
where $r$ - air density, $r = 1.2 \cdot 10^{-5}$  (one magnitude larger than real life). 
### Lift
$$
\vec{F_l} = \vec{u} \cdot l \cdot (|\vec{V}_{xy}| / v ),
$$
$$
\vec{V}_{xy} = \vec{V} \cdot (1, 1, 0) \quad \leftarrow \quad \text{per element multiplication.}
$$
### Wind
$$
\vec{F}_w= \vec{W} \cdot a,
$$
$$
\vec{W} = W \cdot \vec{d},
$$
where $W$ - wind speed, $\vec{d}$ - wind direction.

### Turbulence
$$
\vec{F}_t = \frac{\vec{T}(\vec{L})}{|\vec{T}(\vec{L})|} \cdot t \cdot a \cdot (1 - s),
$$
where $t$ - environmental turbulence multiplier, $\vec{T}$ - turbulence vector, defined by 
$$
\vec{T}(\vec{L}) = \left(
\begin{align}
&2\sin\left(0.5L_x\right)\ +\sin\left(L_x\right)\ +\ 0.5\sin\left(3L_x\right)\ +\ 0.3\ \sin\left(5L_x\right), \\
&2\sin\left(0.3L_y\ +\ 3\right)\ +\ \sin\left(1.1L_y\right)\ +\ 0.5\sin\left(3.34L_y\ +\ 6\right)\ +\ 0.3\sin\left(6L_y\ +\ 18\right) \\
&2\sin\left(0.4L_z\ +\ 12\right)\ +\ \sin\left(0.9L_z\ +\ 5.34\right)\ +\ 0.5\sin\left(4.34L_z\ -\ 9\right)\ +\ 0.15\sin\left(11.5L_z\ +\ 24\right)
\end{align}
\right)
$$
*This function does not have any physics base and was written down on a whim.*


### Resulting Force

Forces are applied through altering arrow's velocity vector:
$$
\vec{V} := \vec{V} + \frac{\vec{F}}{m}.
$$
Forces are applied every tick in order of their introduction in previous sections.