var garray : array[30] integer;

routine main : integer
is
    var larray : array[10] integer;
    garray[2 * 3] := larray;

    print garray[6];

    return 0;
end
