function mysplit(inputstr, sep)
	if sep == nil then
		sep = "%s"
	end

	local t = {}
	for str in string.gmatch(inputstr, "([^" .. sep .. "]+)") do
		table.insert(t, str)
	end

	return t
end

local function getArgs(id)
	local args = mysplit(id)
	if not args or #args < 2 then
		return
	end

	return unpack(args)
end

function script:onWillAppear(btn)
	print("{}: {}", btn, btn:getId())
	local _, key, state = getArgs(btn:getId())

	deck:setTitle(btn:getContext(), key .. ": " .. state)
	deck:showAlert(btn:getContext())
end

function script:onWillDisappear(btn)
	deck:setTitle(":(")
end

function script:onPressDown(btn)
	print("pressing down: {}", btn:getContext())
	local _, key, state = getArgs(btn:getId())

	print("{}, {}, {}", _, key, state)
	if state == "down" then
		desktop:sendKey(key, true)
	else
		desktop:sendKey(key, false)
	end

	deck:showOk(btn:getContext())
end

function script:onPressUp(btn)
	print("pressing up: {}", btn:getContext())
end
