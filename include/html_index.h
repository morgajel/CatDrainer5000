const char* indexPage = R"=====(
<!DOCTYPE html>
<html>
    <head>
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
        <script>
            $(document).ready(function(){
                $("#toggleLaser").click(function(){
                    $.ajax({url: "/toggleLaser", async: false, success: function(result){
                        $("#toggleResult").html(result);
                    }});
                });
            });
            $(document).ready(function(){
                $("#tiltRange").change(function(){
                    $.ajax({url: "/setTilt?tilt=" + $(this).val() 
                    
                    , async: true, success: function(result){
                        $("#toggleResult").html(result);
                    }});
                });
            });

            $(document).ready(function(){
                $("#automated").click(function(){
                    $.ajax({
                        url: "/automate?moves=" + $("#moves").value,
                        async: false,

                        success: function(result){
                            $("#automatedResult").html(result);
                    }});
                });
            });
        </script>
    </head>
    <body>
    <header>
           <h1>Catdrainer 2500</h1>
           Now with more lasers
    </header>
    <nav>
        <a href="/">home</a>
        <a href="/automate?moves=3">Automate 3 moves</a>
        <a href="/settilt?tilt=0">tilt to beginning</a>
        <a href="/settilt?tilt=90">tilt to middle</a>
        <a href="/settilt?tilt=180">tilt to end</a>
        <a href="/status">moves in queue</a>
        <a href="/reset">reboot device</a>
    </nav>
    <section>
        <h2>Basic Controls</h2>
        <label>Tilt Degree</label>
        <input type="range" min="0", max="180" id="tiltRange">
        <button type="button" id="toggleLaser">Toggle Laser</button>
    </section>
    <aside>
        <h2> Current State</h2>
        <div id="toggleResult">Laser disarmed</div>
        <div id="automatedResult"></div>
    </aside>
    <section>
        <h2> Automation Testing</h2>
        <label for="quantity">Moves</label>
        <input type="number" id="moves" name="quantity" min="1" max="20" value="10"/>
        <button type="button" id="automated">execute automation</button>
        </form>
    </section>

</body></html>

)=====";