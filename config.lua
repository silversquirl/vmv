--[[

config.lua

This file allows you to specify which visualiser to use as well as various other options.

]]--

-- Some custom config options provided by the sample visualiser, in vis.lua
bars_config = {}
bars_config['padding'] = 0.1
bars_config['color'] = {1, 0, 1}
bars_config['color_alt'] = {1, 0, 0}


config = {}

-- This option should specify a function to use as a visualiser.
-- See vis.lua for more details.
config['visualiser'] = require("vis")

-- This option sets a key which can be used to close the window.
config['close_key'] = 'x'

-- This options sets the monitor on which the visualiser will appear.
config['monitor'] = 0

-- This option sets the width of the visualiser window when it opens.
config['width'] = 512

-- This option sets the height of the visualiser window when it opens.
config['height'] = 256

-- This option sets the x position of the visualiser window when it opens.
config['x'] = 50

-- This option sets the y position of the visualiser window when it opens.
config['y'] = 50

-- If set to true, the visualiser window will always appear above all others.
config['always_on_top'] = true

return config
