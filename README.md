# 2D Fluid Simulation
2D Fluid simulation using smoothed-particle hydrodynamics (SPH)in C++.

<img width="947" alt="image" src="https://github.com/OHashish/Fluid-Simulation/assets/95832061/ea7bb85e-b715-46fe-8cf4-eee7cfbffc2d">


# Forces

## Pressure Force
$$  \mathbf{f}_{i, \text{pressure}} = -\sum_j m_j \frac{p_i + p_j}{2 \rho_j} \nabla W(\mathbf{r}_i - \mathbf{r}_j, h)$$

$$ \text{where} \hspace{1cm} p = k(\rho - \rho_0)$$

## Viscosity Force
$$\mathbf{f}_{i, \text{viscosity}} = \mu \sum_j m_j \frac{\mathbf{v}_j - \mathbf{v}_i}{\rho_j} \nabla^2 W(\mathbf{r}_i - \mathbf{r}_j, h)$$

## Surface Tension Force
$$ \mathbf{f}_{i, \text{surface tension}} = -\sigma \kappa_i \mathbf{n}_i = -\sigma \nabla^{2}c_s \frac{\mathbf{n}}{\|\mathbf{n}\|}$$
 
$$ \text{where} \hspace{1cm} c_s(\mathbf{r}) = \sum_j \frac{m_j}{\rho_j} W(\mathbf{r} - \mathbf{r}_j, h)$$

$$ \text{and} \hspace{1cm} \mathbf{n} = \nabla c_s$$

## Total Force 

$$\mathbf{f}_ {\text{total force}} =  \mathbf{f}_ {\text{pressure force}} + \mathbf{f}_ {\text{viscosity force}} + \mathbf{f}_ {\text{surface tension force}} + \frac{m(\text{gravity})}{\rho}$$

# Kernels

The following kernels were using for the particle simulation:
### Poly6 kernel

$$W_{\text{poly6}}(r, h) = \frac{315}{64\pi h^9}
\begin{cases}
  (h^2 - r^2)^3 & \text{for } 0 \leq r \leq h \\
  0 & \text{for } r > h
\end{cases}$$

Poly6 kernel was used to calcuate the density of the particles and to calculate the surface tension. It can also be used to calculate the pressure from the other particles but the spiky kernel is preferred in this sitution as poly6 is less stable and particles tend to form clusters when using it .

### Spiky kernel

$$ W_{\text{spiky}}(r, h) = \frac{15}{\pi h^6}
\begin{cases}
  (h - r)^3 & \text{for } 0 \leq r \leq h \\
  0 & \text{for } r > h
\end{cases}$$

The gradient of the spiky kernel was used to calculate the pressure for each partcile.

### Viscosity kernel

$$ W_{\text{viscosity}}(r, h) = \frac{15}{2\pi h^3}
\begin{cases}
  -\frac{r^3}{2h^3} + \frac{r^2}{h^2} + \frac{h}{2r} - 1 & \text{for } 0 \leq r \leq h \\
  0 & \text{for } r > h
\end{cases} $$

The laplacian of the viscosity kernel was used for the viscosity calculation for each particle.

# Integration Scheme

### Leapfrog integration scheme

$$ v_{i+1/2} = v_i + a_i(\frac{\Delta t}{2}) $$

$$ x_{i+1} = x_i + v_{i+1/2} \Delta t $$

$$ v_{i+1} = v_{i+1/2} + a_{i+1}(\frac{\Delta t}{2}) $$

# Installation

To install and run this project, you need to have the following requirements:

  - Qt 5.15 or higher
  
  - OpenGL
  
  - GLM library

Once you have installed the requirements, you can clone this repository or download the source code as a zip file.

You can either :

Open the .pro file using Qt Creator and run it using the IDE 

Or 

Install Qt

Then, open a terminal and navigate to the project directory. To compile the project, run the following commands:

```
qmake
make
```
This will generate an executable file named app in the same directory. To run the project, simply execute the file by typing:

```
./Fluid.app
```
You should see a window with the fluid simulation and a GUI.

Note: the spinboxes are there to refine parametere when increasing or decreasing particles. Choosing the wrong parameters will make the simulation unstable.

### References
- Schuermann, Lucas V. (Jul 2017). Implementing SPH in 2D. Writing. https://lucasschuermann.com/writing/implementing-sph-in-2d.
- Müller, M., Charypar, D., & Gross, M. (Jul 2003). Particle-Based Fluid Simulation for Interactive Applications. Writing. https://matthias-research.github.io/pages/publications/sca03.pdf. 
