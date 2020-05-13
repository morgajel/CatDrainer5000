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
                $("#automated").click(function(){
                    $.ajax({url: "/automated", async: false, success: function(result){
                        $("#automatedResult").html(result);
                    }});
                });
            });
        </script>
    </head>
    <body>
        <b>Catdrainer 5000:</b> now with more Servos.
        <br/>
        <button type="button" id="toggleLaser">Toggle Laser</button> <span id="toggleResult"></span>
        <br/>
        <button type="button" id="automated">automated run</button> <span id="automatedResult"></span>
</body></html>

)=====";