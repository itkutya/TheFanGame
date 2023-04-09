<?php
$host = "localhost";
$user = "id19916612_thefangameadmindb";
$password = "jOsW@15=7W{{csr<";
$db = "id19916612_thefangamedb";

$mysqli = new mysqli($host, $user, $password, $db);
if($mysqli->connect_error)
    echo $mysqli->connect_error;
else
{
    if(isset($_POST['username']))
    {
        $name = $_POST['username'];
        $resoult = $mysqli->query($query);
        if($resoult)
        {
            $resoult = $mysqli->query($query);
            if($resoult)
                echo "Success.";
            else
                echo "Failed.";
        }
    }
}
exit();
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="icon" type="image/x-icon" href="favicon.ico">
    <title>Register to the game.</title>
</head>
<body>
</body>
</html>