function sleep (a) 
    local sec = tonumber(os.clock() + a); 
    while (os.clock() < sec) do 
    end 
end

print('Running C++ code in Lua!')

sleep(3)
Run()