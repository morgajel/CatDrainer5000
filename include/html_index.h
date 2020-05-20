<!--const char* indexPage = R"=====(-->
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
           <h1>Catdrainer 2500</h1>

           <nav>
            <input type="range" min="0", max="180" >

           </nav>
    </section>
        <br/>
        <button type="button" id="toggleLaser">Toggle Laser</button> <span id="toggleResult"></span>
        <br/>
        <form>
        <label for="quantity">Moves (between 1 and 20):</label>
        <input type="number" id="moves" name="quantity" min="1" max="20" value="10"/>
        <button type="button" id="automated">automated run</button> <span id="automatedResult"></span>
        </form>
    
</body></html>

)=====";