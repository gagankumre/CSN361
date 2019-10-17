import csv
reader = csv.DictReader(open("packet_capture.csv"))
tout = 0
tinc = 0
out = 0
inc = 0
inflow = dict()
outflow = dict()
distdstIP = dict()
for raw in reader:
	key = raw["Source"] + raw["Destination"] + raw["Protocol"] + raw["Source Port"] + raw["Destination Port"] 
	distdstIP[raw["Destination"]] = 0
	if key not in inflow:
		inflow[key] = 900
		outflow[key] = 0
	outflow[key] = max(outflow[key], float(raw["Time"]))
	inflow[key] = min(inflow[key], float(raw["Time"]))
	if raw["Source"] == "10.43.1.228":
		out += 1
		tout += int(raw["Length"])
	else:
		inc += 1
		tinc += int(raw["Length"])
toflow = len(outflow) + len(inflow)
todura = 0
topack = out + inc
for k in inflow:
	todura += (outflow[k] - inflow[k])
print(len(distdstIP))
print("Average Packet Size : {}".format((tinc + tout) / topack))
print("Average Flow Duration : {}".format((todura) / toflow))
print("Average Number of Packets Sent per Flow : {}".format((out) / toflow))
print("Average Number of Packets Received per Flow : {}".format((inc) / toflow))
print("Average Number of Bytes Sent per Flow : {}".format((tout) / toflow))
print("Average Number of Bytes Received per Flow : {}".format((tinc) / toflow))
print("Average Ratio of Incoming Packets to Outgoing Packets : {}".format((out) / inc))
print("Average Ratio of Incoming Bytes to Outgoing Bytes : {}".format((tout) / tinc))
print("Average Time Interval b/w Packets Sent : {}".format((894.272579381) / tout))
print("Average Ratio of Connections to Number of Destination IPs : {}".format((50.0) / len(distdstIP)))
