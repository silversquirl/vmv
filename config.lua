--[[

config.lua

This file allows you to specify which visualiser to use as well as various other options.

]]--

-- Some custom config options provided by the sample visualiser, in vis.lua
return {
  -- This option should specify a function to use as a visualiser.
  -- See vis.lua for more details.
  visualiser = require("vis")({
    padding = 0.1,
    color = {1, 0, 1},
    color_alt = {1, 0, 0},
  }),

  -- This option sets a key which can be used to close the window.
  close_key = 'x',

  -- This options sets the monitor on which the visualiser will appear.
  monitor = 0,

  -- This option sets the width of the visualiser window when it opens.
  width = 512,

  -- This option sets the height of the visualiser window when it opens.
  height = 256,

  -- This option sets the x position of the visualiser window when it opens.
  x = 50,

  -- This option sets the y position of the visualiser window when it opens.
  y = 50,

  -- If set to true, the visualiser window will always appear above all others.
  always_on_top = true,

  -- Sets an FPS cap. If not present, this will be automatically set according to your monitor refresh rate.
  --fps_cap = 60,

  -- Sets the number of bars to appear. Defaults to 10.
  bars = 15,
}
