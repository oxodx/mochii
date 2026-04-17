function fetch(name, url, tag, path)
    if not os.isdir(path) then
        os.execute("git clone --depth=1 --branch " .. tag .. " " .. url .. " " .. path)
    end
end
