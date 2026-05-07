<?php

header("Content-Type: application/json; charset=UTF-8");
header("Access-Control-Allow-Origin: *");

define("TABLE_NAME", "scores");
define("DB_NAME", "scores.db");
define("PAGE_SIZE", 10);

// method to create dp path
function createDbPath() {
    $db_path = __DIR__ . "/" . DB_NAME;
    try {
        $pdo = new PDO('sqlite:' . $db_path);
        $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);        // Exceptions on
        $pdo->setAttribute(PDO::ATTR_DEFAULT_FETCH_MODE, PDO::FETCH_ASSOC); // fetch mit asssoziativem Array
        $pdo->exec("PRAGMA busy_timeout = 5000");
        return $pdo;
    } catch (PDOException $e) {
        http_response_code(500);
        echo json_encode(["error" => "Database connection failed: " . $e->getMessage()]);
        exit;
    }
}

function createTable($pdo, $tableName = TABLE_NAME) {
    $createSQL = <<<SQL
    CREATE TABLE IF NOT EXISTS $tableName (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT NOT NULL,
        score INTEGER NOT NULL,
        created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    );
    SQL;
    try {
        $table = $pdo->exec($createSQL);
        if ($table === false) {
            throw new PDOException("Table creation failed");
        }
        return true;
    } catch (PDOException $e) {
        http_response_code(500);
        echo json_encode(["error" => "Table creation failed: " . $e->getMessage()]);
        exit;
    }
}

function getScores($pdo, $player = null, $limit = null, $offset = null, $tableName = TABLE_NAME) {
    // create sql query dynamically based on parameters
    $sql = "SELECT * FROM $tableName";
    if ($player) {
        $sql .= " WHERE name = :name";
    }
    $sql .= " ORDER BY score DESC";
    if ($limit) {
        $sql .= " LIMIT :limit";
    }
    if ($offset) {
        $sql .= " OFFSET :offset";
    }

    // prepare sql query to prevent SQL injection
    $stmt = $pdo->prepare($sql);
    // bind parameters
    if ($player) {
        $stmt->bindValue(':name', $player, PDO::PARAM_STR);
    }
    if ($limit) {
        $stmt->bindValue(':limit', $limit, PDO::PARAM_INT);
    }
    if ($offset) {
        $stmt->bindValue(':offset', $offset, PDO::PARAM_INT);
    }
    // execute sql query
    try {
        $stmt->execute();
        return $stmt->fetchAll(PDO::FETCH_ASSOC);
    } catch (PDOException $e) {
        http_response_code(500);
        echo json_encode(["error" => "Failed to get scores: " . $e->getMessage()]);
        exit;
    }
}

function addScore($pdo, $name, $score, $tableName = TABLE_NAME) {
    try {
        $sql = "INSERT INTO $tableName (name, score) VALUES (:name, :score)";
        $stmt = $pdo->prepare($sql);
        $stmt->bindValue(':name', $name, PDO::PARAM_STR);
        $stmt->bindValue(':score', $score, PDO::PARAM_INT);
        $stmt->execute();
        return ["success" => true, "id" => $pdo->lastInsertId()];
    } catch (PDOException $e) {
        http_response_code(500);
        echo json_encode(["error" => "Failed to add score: " . $e->getMessage()]);
        exit;
    }
}
function getTotalPlayerScore($pdo, $name, $tableName = TABLE_NAME) {
    $sql = "SELECT SUM(score) FROM $tableName WHERE name = :name";
    $stmt = $pdo->prepare($sql);
    $stmt->bindValue(':name', $name, PDO::PARAM_STR);
    $stmt->execute();
    return $stmt->fetch(PDO::FETCH_ASSOC);
}

function deleteScore($pdo, $id = null, $player = null, $tableName = TABLE_NAME) {
    $sql = "DELETE FROM $tableName";
    $conditions = [];
    
    if ($player) $conditions[] = "name = :name";
    if ($id) $conditions[] = "id = :id";
    $sql .= " WHERE " . implode(" AND ", $conditions);
    // error_log("Sql: " . $sql);
    
    $stmt = $pdo->prepare($sql);
    if ($player) $stmt->bindValue(':name', $player, PDO::PARAM_STR);
    if ($id) $stmt->bindValue(':id', $id, PDO::PARAM_INT);

    try {
        $stmt->execute();
        return true;
    } catch (PDOException $e) {
        http_response_code(500);
        echo json_encode(["error" => "Failed to delete score: " . $e->getMessage()]);
        exit;
    }
}

$method = $_SERVER['REQUEST_METHOD'];
$request_uri = $_SERVER['REQUEST_URI'];

// echo "Request Path: " . $request_uri;
$pdo = createDbPath();
createTable($pdo);

switch($method) {
    case 'GET':
        
        // check if the request has a player parameter
        $player = $_GET['player'] ?? null;
        if ($player) {
            $totalScore = getTotalPlayerScore($pdo, $player);
            echo json_encode($totalScore);
            exit;
        } else {
            // check if the request has a page parameter
            $page = (int) ($_GET['page'] ?? 1);
            $offset = ($page - 1) * PAGE_SIZE;
            $limit = PAGE_SIZE;
            $scores = getScores($pdo, limit: $limit, offset: $offset);
            echo json_encode($scores);
            break;
        }

    case 'POST':
        // add a new score
        $name = $_POST['name'] ?? null;
        $score = $_POST['score'] ?? null;

        // Fallback for JSON input
        if ($name === null || $score === null) {
            // print just to the server console
            // error_log("Json Resolution");
            // error_log(file_get_contents('php://input'));
            $json = json_decode(file_get_contents('php://input'), true);
            // error_log("Json: " . $json);
            $name = $json['name'] ?? $name;
            $score = $json['score'] ?? $score;
        }

        // error_log("Name: " . $name);
        // error_log("Score: " . $score);

        if ($name === null || $score === null) {
            http_response_code(400);
            echo json_encode(["error" => "Name and score are required"]);
            exit;
        }

        $result = addScore($pdo, $name, $score);
        echo json_encode($result);
        break;

    case 'DELETE':
        // delete a score
        // check if the request has an id or player parameter
        $id = $_GET['id'] ?? null;
        $player = $_GET['player'] ?? null;

        // Fallback for JSON input
        if ($id === null || $player === null) {
            // error_log("Json Resolution");
            $json = json_decode(file_get_contents('php://input'), true);
            $id = $json['id'] ?? $id;
            $player = $json['player'] ?? $player;
        }

        // error_log("Id: " . $id);
        // error_log("Player: " . $player);

        if (!$id && !$player) {
            http_response_code(400);
            echo json_encode(["error" => "Id or player is required"]);
            exit;
        }

        $deleted = deleteScore($pdo, id: $id, player: $player);
        echo json_encode($deleted);
        break;
    default:
        http_response_code(405);
        echo json_encode(["error" => "Method not allowed"]);
        break;
}

?>