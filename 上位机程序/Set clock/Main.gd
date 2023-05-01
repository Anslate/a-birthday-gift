extends Control

var auto = false
var port = []
var count = 0

func _ready():
	var output=[]
	OS.execute("./uart.exe",["-p"],true,output)
	var a = output[0].split("\r\n",false)
	for i in a:
		$B/C.add_item(i)
	if(!$B/C.text.empty()):
		$B/target_time_W.disabled=false
		$B/now_time_W.disabled=false
		$B/now_time_R.disabled=false
		$B/target_time_R.disabled=false
	
func _process(delta):
	if(auto):
		var time = Time.get_datetime_dict_from_system()
		$W/s.value = time["second"]
		$W/min.value = time["minute"]
		$W/h.value = time["hour"]
		$W/d.value = time["day"]
		$W/mon.value = time["month"]
		$W/w.value = time["weekday"]
		$W/y.value = int(str(time["year"]).right(2))
		pass 
	if(count==39):
		the_egg()

func the_egg():
	$W.visible=false
	$T.visible=false
	$B.visible=false
	$auto.visible=false
	$E.visible=true

func _on_auto_toggled(button_pressed):
	$W/s.editable=!button_pressed
	$W/min.editable=!button_pressed
	$W/h.editable=!button_pressed
	$W/d.editable=!button_pressed
	$W/mon.editable=!button_pressed
	$W/w.editable=!button_pressed
	$W/y.editable=!button_pressed
	auto=button_pressed


func _on_now_time_W_pressed():#写入现在时间
	var data
	data="%02d%02d%02d%02d%02d%02d%02d%02d" % [$W/s.value,$W/min.value,$W/h.value,$W/d.value,$W/mon.value,$W/w.value,$W/y.value,00]
	print($B/C.text,data)
	OS.execute("./uart.exe",["-CTX",$B/C.text,data],false)

func _on_now_time_R_pressed():#读取现在时间
	auto=false
	$auto.pressed=false
	var output = []
	OS.execute("./uart.exe",["-CRX",$B/C.text],true,output)
	var data = output[0].strip_edges().split("-")
	$W/s.value = int(data[0])
	$W/min.value = int(data[1])
	$W/h.value = int(data[2])
	$W/d.value = int(data[3])
	$W/mon.value = int(data[4])
	$W/w.value = int(data[5])
	$W/y.value = int(data[6])

func _on_target_time_W_pressed():#写入目标时间
	count += 1
	var data
	data="%02d%02d%02d%02d%02d%02d%02d%02d" % [$W/s.value,$W/min.value,$W/h.value,$W/d.value,$W/mon.value,$W/w.value,$W/y.value,00]
	print($B/C.text,data)
	OS.execute("./uart.exe",["-MTX",$B/C.text,data],false)
	
func _on_target_time_R_pressed():#读取目标时间
	auto=false
	$auto.pressed=false
	var output = []
	OS.execute("./uart.exe",["-MRX",$B/C.text],true,output)
	var data = output[0].strip_edges().split("-")
	$W/s.value = int(data[0])
	$W/min.value = int(data[1])
	$W/h.value = int(data[2])
	$W/d.value = int(data[3])
	$W/mon.value = int(data[4])
	$W/w.value = int(data[5])
	$W/y.value = int(data[6])

func _on_C_item_selected(index):
	$B/target_time_W.disabled=false
	$B/now_time_W.disabled=false
	$B/now_time_R.disabled=false
	$B/target_time_R.disabled=false
