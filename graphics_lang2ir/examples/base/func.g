routine fact(n : integer) : integer
is
    if (n <= 1)
    then
        return 1;
    end

    return n * fact(n - 1);
end

routine main : integer
is
    print fact(?);
    return 0;
end
