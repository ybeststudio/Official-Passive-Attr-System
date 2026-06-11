# In `__init__`, extend the if-statement with:
		if app.ENABLE_PASSIVE_ATTR:
			self.interface.RegisterPassiveAttrHandler()

# Add anywhere in the GameWindow class:
	if app.ENABLE_PASSIVE_ATTR:
		def RegisterPassiveAttrHandler(self):
			if self.interface:
				self.interface.RegisterPassiveAttrHandler()
