return function (config)
  return function (bars)
    table.remove(bars, 1)
    local width = 2 / #bars
    local p1 = {}
    local p2 = {}
    local p3 = {}
    local p4 = {}

    for i, m in ipairs(bars) do
      p1[1] = (i - 1) * width - 1 + config['padding'] / 2
      p1[2] = -m / vmv.bar_max

      p2[1] = (i - 1) * width - 1 + config['padding'] / 2
      p2[2] = m / vmv.bar_max

      p3[1] = i * width - 1 - config['padding'] / 2
      p3[2] = m / vmv.bar_max

      p4[1] = i * width - 1 - config['padding'] / 2
      p4[2] = -m / vmv.bar_max

      if i % 2 == 0 then
        vmv.draw.rectangle(p1, p2, p3, p4, config['color'])
      else
        vmv.draw.rectangle(p1, p2, p3, p4, config['color_alt'])
      end
    end
  end
end
