# Converts desmos polygons into vertices for this project

poly = input("desmos: ")
color = "SECONDARY_COLOR"
print()
raw = poly[39:len(poly)-14].split(",\\left")
# print(raw)
print()
result = "{"
for rawpair in raw:
  pair = rawpair[6:][:-7].split(",")
  p0n = "-" if rawpair[1] == "-" else ""
  p1n = "-" if pair[1].replace("\\frac", "").split("{")[0] == "-" else ""
  pair0Split = pair[0].split("{")[1:]
  pair1Split = pair[1].replace("\\frac", "").split("{")[1:]
  # print(pair1Split[0])
  result += "vtx(" + p0n
  if pair[0] == "0":
    result += "0, "
  else:
    result += (pair0Split[0][:len(pair0Split[0])-2] + "*") if pair0Split[0][:len(pair0Split[0])-2] != "" else ""
    result += "LENGTH"
    result += "/" + pair0Split[1][:len(pair0Split[1])-1] + ", "

  result += p1n
  if pair[1].replace("\\frac", "") == "0":
    result += "0, "
  else:
    result += (pair1Split[0][:len(pair1Split[0])-2] + "*") if pair1Split[0][:len(pair1Split[0])-2] != "" else ""
    result += "WIDTH"
    result += "/" + pair1Split[1][:len(pair1Split[1])-1] + ", "
  result += color + "), "
  # print(result + "\n")

print(result[:len(result)-2] + "},")