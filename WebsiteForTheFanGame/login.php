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
    if(isset($_POST['username']) && isset($_POST['password']))
    {
        $name = $_POST['username'];
        $pw = $_POST['password'];
        
        $query = "SELECT * FROM TheFanGameAccountsInfo INNER JOIN TheFanGameAccounts ON TheFanGameAccountsInfo.Username = TheFanGameAccounts.Username WHERE TheFanGameAccounts.Username = '$name'";  
        $resoult = $mysqli->query($query);
        if($resoult->num_rows)
        {
            $row = $resoult->fetch_array(MYSQLI_ASSOC);
            $salt = $row["Salt"];
            if(($pw - ($salt * 352)) == $row["Password"])
            {
                echo "Success.#";
                echo $row["Level"]."#";
                echo $row["CurrentXP"]."#";
                echo $row["MaxXP"]."#";
                echo $row["Currency"];
            }
            else
                echo "Incorrect password.";
        }
        else
            echo "Query failed.";
    }
    else
        echo "Values not set.";
}
exit();
?>