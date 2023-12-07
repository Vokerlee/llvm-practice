routine main : integer
is
    var x1 : integer is 5;

    if (x1 > 20)
    then
        print 1;
    else
        x1 := 20;
        if (x1 != 22)
        then
            print 5;
        else
            print 777;
        end
    end

    print x1;

    return 0;
end
