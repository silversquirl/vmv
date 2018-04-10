function bar_vis(bars)
  table.remove(bars, 1)
  width = 2 / #bars
  p1 = {}
  p2 = {}
  p3 = {}
  p4 = {}

  for i, m in ipairs(bars) do
    p1[1] = (i - 1) * width - 1 + bars_config['padding'] / 2
    p1[2] = -m / vmv.bar_max

    p2[1] = (i - 1) * width - 1 + bars_config['padding'] / 2
    p2[2] = m / vmv.bar_max

    p3[1] = i * width - 1 - bars_config['padding'] / 2
    p3[2] = m / vmv.bar_max

    p4[1] = i * width - 1 - bars_config['padding'] / 2
    p4[2] = -m / vmv.bar_max

    if i % 2 == 0 then
      vmv.draw.rectangle(p1, p2, p3, p4, bars_config['color'])
    else
      vmv.draw.rectangle(p1, p2, p3, p4, bars_config['color_alt'])
    end
  end
end

bars_config = {}
bars_config['padding'] = 0.1
bars_config['color'] = {1, 0, 1}
bars_config['color_alt'] = {1, 0, 0}

config = {}
config['visualiser'] = bar_vis
config['close_key'] = 'x'
config['monitor'] = 0
config['width'] = 500
config['center'] = true

return config
