# Add the following to this file:
if app.ENABLE_PASSIVE_ATTR:
	window["height"] = window["height"]
	window["children"][0]["height"] = window["children"][0]["height"]
	window["children"][0]["children"] = window["children"][0]["children"] + [
		{ "name" : "passive_attr", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_PASSIVE_ATTR1, "text_horizontal_align" : "left", "x" : 28 + 360 + 35, "y" : 75 + 280, },
		{ "name" : "passive_attr", "type" : "text", "text" : uiScriptLocale.KEYCHANGE_PASSIVE_ATTR2, "text_horizontal_align" : "left", "x" : 28 + 360 + 35, "y" : 75 + 300, },
	]
