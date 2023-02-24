<?php
$host = "localhost";
$user = "id19916612_thefangameadmindb";
$password = "jOsW@15=7W{{csr<";
$db = "id19916612_thefangamedb";

$mysqli = new mysqli($host, $user, $password, $db);
if(mysqli_connect_errno())
    echo $mysqli->connect_error;
else
{
    if(isset($_POST['username']) && isset($_POST['password']) && isset($_POST['random']))
    {
        $random = $_POST['random'];
        $name = $_POST['username'];
        if($random != 0)
        {
            $query = "SELECT * FROM TheFanGameAccountsInfo INNER JOIN TheFanGameAccounts ON TheFanGameAccountsInfo.Username = TheFanGameAccounts.Username WHERE TheFanGameAccounts.Username = '$name'";  
            $resoult = $mysqli->query($query);
            if($resoult->num_rows)
            {
                if($row = $resoult->fetch_array(MYSQLI_ASSOC))
                {
                    if($row["LoginBuffer"] != 0)
                    {
                        if($row["LoginBuffer"] === $random)
                        {
                            echo "Success.#";
                            echo $row["Level"]."#";
                            echo $row["CurrentXP"]."#";
                            echo $row["MaxXP"];
                        }
                    }
                    else
                    {
                        $pw = $_POST['password'];
                        if($row['Password'] === $pw)
                        {
                            $query2 = "UPDATE `TheFanGameAccounts` SET `LoginBuffer` = '$random' WHERE `TheFanGameAccounts`.`Username` = '$name' AND  `TheFanGameAccounts`.`Password` = '$pw'";
                            $resoult2 = $mysqli->query($query2);
                            echo "Success.#";
                            echo $row["Level"]."#";
                            echo $row["CurrentXP"]."#";
                            echo $row["MaxXP"];
                        }
                    }
                }
            }
            else
                echo "Failed.";
        }
        else
        {
            $pw = $_POST['password'];
            $query = "SELECT * FROM TheFanGameAccountsInfo INNER JOIN TheFanGameAccounts ON TheFanGameAccountsInfo.Username = TheFanGameAccounts.Username WHERE TheFanGameAccounts.Username = '$name' AND TheFanGameAccounts.Password = '$pw'";  
            $resoult = $mysqli->query($query);
            if($resoult->num_rows)
            {
                echo "Success.#";
                if($row = $resoult->fetch_array(MYSQLI_ASSOC))
                {
                    echo $row["Level"]."#";
                    echo $row["CurrentXP"]."#";
                    echo $row["MaxXP"];
                }
            }
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
    <title>Login to the game.</title>
</head>
<body>
</body>
</html>