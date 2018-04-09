function bar_vis(bars)
  width = 2 / #bars
  p1 = {}
  p2 = {}
  p3 = {}
  p4 = {}
  color = {}

  color[1] = 1
  color[2] = 0
  color[3] = 1

  for i, m in ipairs(bars) do
    p1[1] = (i - 1) * width - 1
    p1[2] = -m / vmv.bar_max

    p2[1] = (i - 1) * width - 1
    p2[2] = m / vmv.bar_max

    p3[1] = i * width - 1
    p3[2] = m / vmv.bar_max

    p4[1] = i * width - 1
    p4[2] = -m / vmv.bar_max

    vmv.draw.rectangle(p1, p2, p3, p4, color)
  end
end
config = {}
config['visualiser'] = bar_vis


return config
