var gv1 : integer;

routine main : integer
is
    var v1 : integer is 2 * 6;
    gv1 := 5;
    v1 := gv1 * v1;

    print v1;

    return 0;
end
