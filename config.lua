function bar_vis(bars)
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

    vmv.draw.rectangle(p1, p2, p3, p4, bars_config['color'])
  end
end

bars_config = {}
bars_config['color'] = {1, 0, 0}
bars_config['padding'] = 0.1

config = {}
config['visualiser'] = bar_vis

return config
