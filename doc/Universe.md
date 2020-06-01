# Procedural Universe

U = universe seed
S = seed
x = galaxy cell x
y = galaxy cell y
z = galaxy cell z

Universe -> Galaxies -> Solar Systems -> Planets -> Moons


## Galaxy

Galaxies are placed in a 3D grid where the center cell is at (0, 0, 0).

### Get the current galaxy

S_galaxy = [U, x, y, z]


## Solar system

The galaxy grid cell is further divided into the solar systems grid where solar systems exists. Each solar systems grid cell can have 0+ solar systems.

The solar system formations can be generated using a noise function to get the twisting/arms effects for the galaxy.

A solar system belongs to a galaxy and can have 0+. Each star can have 0+ planets and each planet can have 0+ moons.

IDs:
    SUN = 0
    PLANET = 1

### Get the current solar system grid cell

S_solar = [U, x_g, y_g, z_g, x_s, y_s, z_s]

### Get sun # in the current solar system

S_sun1 = [U, x_g, y_g, z_g, x_s, y_s, z_s, SUN, #]

### Get planet # in the current solar system

S_sun1 = [U, x_g, y_g, z_g, x_s, y_s, z_s, PLANET, #]