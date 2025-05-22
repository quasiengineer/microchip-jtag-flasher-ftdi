project('microchip-jtag-flasher', 'c')

# includes
incdir = include_directories(
    'libs/DirectC', 'libs/DirectC/JTAG', 'libs/DirectC/G3Algo', 'libs/DirectC/SPIFlash', 'libs/DirectC/G4Algo', 'libs/DirectC/G5Algo', 'libs/DirectC/RTG4Algo',
)

# sources
sources = ['main.c']
libs_sources = run_command('find', 'libs', '-name', '*.c').stdout().split()

# libs
ftdi_dep = dependency('libftdi1', static: true, required: true)

executable('microchip-jtag-flasher', sources + libs_sources, include_directories: incdir, dependencies: [ftdi_dep])