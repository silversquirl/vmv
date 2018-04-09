function visualise(bars)
  width = 2 / #bars
  output = {}
  for i,m in ipairs(bars) do
    table.insert(output, (i-1)*width-1)
    table.insert(output, -m/1024)

    table.insert(output, (i-1)*width-1)
    table.insert(output, m/1024)

    table.insert(output, i*width-1)
    table.insert(output, m/1024)

    table.insert(output, i*width-1)
    table.insert(output, -m/1024)
  end
  return output
end
