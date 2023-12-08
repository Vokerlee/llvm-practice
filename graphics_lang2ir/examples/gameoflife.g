var WINDOW_HEIGHT : integer;
var WINDOW_WIDTH  : integer;

var BUFFER_1 : array[300000] integer;
var BUFFER_2 : array[300000] integer;

routine gameoflife_init
is
    var x : integer is 0;

    while x < WINDOW_WIDTH * WINDOW_HEIGHT
    loop
        BUFFER_1[x] := (sgl_rand() % 2 == 1) * 4278233651; // 0xFF00AA33 = 4278233651
        x := x + 1;
    end

    sgl_update(BUFFER_1);

    return;
end

routine is_cell_alive(x : integer, y : integer) : integer
is
    var x : integer is (x + WINDOW_WIDTH)  % WINDOW_WIDTH;
    var y : integer is (y + WINDOW_HEIGHT) % WINDOW_HEIGHT;

    return (BUFFER_1[y * WINDOW_WIDTH + x] and 4278190080) != 0; // 0xFF000000 = 4278190080
end

routine get_alive_neighbours(x : integer, y : integer) : integer
is
    return is_cell_alive(x - 1, y - 1) + is_cell_alive(x, y - 1) + is_cell_alive(x + 1, y - 1) + is_cell_alive(x + 1, y) +
           is_cell_alive(x + 1, y + 1) + is_cell_alive(x, y + 1) + is_cell_alive(x - 1, y + 1) + is_cell_alive(x - 1, y);
end

routine sync_buffers
is
    var x : integer is 0;

    while x < WINDOW_HEIGHT * WINDOW_WIDTH
    loop
        BUFFER_1[x] := BUFFER_2[x];
        x := x + 1;
    end

    return;
end

routine main : integer
is
    WINDOW_HEIGHT := 400;
    WINDOW_WIDTH  := 400;

    var n_neighbours : integer is 0;
    var alive_state : integer is 0;

    var y : integer is 0;
    var x : integer is 0;

    sgl_initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
    gameoflife_init();

    var buffer_counter : integer is 0;

    while (buffer_counter < 10000)
    loop
        y := 0;

        while y < WINDOW_HEIGHT
        loop
            x := 0;

            while x < WINDOW_WIDTH
            loop
                n_neighbours := get_alive_neighbours(x, y);
                alive_state  := is_cell_alive(x, y) != 0;

                if (alive_state != 0)
                then
                    if ((n_neighbours == 2) or (n_neighbours == 3)) != 0
                    then
                        alive_state := 1;
                    else
                        alive_state := 0;
                    end
                else
                    if n_neighbours == 3
                    then
                        alive_state := 1;
                    else
                        alive_state := 0;
                    end
                end

                // print y * WINDOW_WIDTH + x;
                BUFFER_2[y * WINDOW_WIDTH + x] := alive_state * 4278233651; // 0xFF00AA33 = 4278233651

                // print 111111;
                x := x + 1;
            end

            y := y + 1;
        end

        // print 99999;
        sgl_update(BUFFER_2);
        sync_buffers();

        buffer_counter := buffer_counter + 1;
    end

    sgl_close();

    return 0;
end
