return function (config)
  return function (bars)
    local sum = 0
    for k, v in ipairs(bars) do
      if v > sum then sum = v end
    end

    sum = sum / vmv.bar_max * 2 - 1

    vmv.draw.triangle({-1, -1}, {0, sum}, {1, -1}, config['color'])
  end
end
