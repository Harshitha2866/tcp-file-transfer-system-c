const express = require("express");
const multer = require("multer");
const cors = require("cors");
const path = require("path");
const fs = require("fs");

const app = express();
const PORT = 3000;

app.use(cors());
app.use(express.json());

// Upload folder
const uploadFolder = path.join(__dirname, "..", "uploads");
console.log("Upload folder:", uploadFolder);
console.log("Files:", fs.existsSync(uploadFolder) ? fs.readdirSync(uploadFolder) : "Folder not found");
// Configure multer
const storage = multer.diskStorage({
  destination: (req, file, cb) => {
    cb(null, uploadFolder);
  },
  filename: (req, file, cb) => {
    cb(null, file.originalname);
  }
});

const upload = multer({ storage });

// Upload API
app.post("/upload", upload.single("file"), (req, res) => {
  res.json({
    success: true,
    message: "File uploaded successfully!"
  });
});

// List Files API
app.get("/files", (req, res) => {
  fs.readdir(uploadFolder, (err, files) => {
    if (err) {
      return res.status(500).json({
        success: false
      });
    }

    res.json(files);
  });
});

// Download API
app.get("/download/:filename", (req, res) => {
  const filePath = path.join(uploadFolder, req.params.filename);
  res.download(filePath);
});

// Delete API
app.delete("/delete/:filename", (req, res) => {

  const filePath = path.join(uploadFolder, req.params.filename);

  fs.unlink(filePath, (err) => {

    if (err) {
      return res.status(500).json({
        success: false,
        message: "Unable to delete file."
      });
    }

    res.json({
      success: true,
      message: "File deleted successfully!"
    });

  });

});

// Serve frontend
app.use(express.static(path.join(__dirname, "..", "client")));

app.get("/", (req, res) => {
  res.sendFile(path.join(__dirname, "..", "client", "index.html"));
});

// Start Server
app.listen(PORT, () => {
  console.log(`Server running at http://localhost:${PORT}`);
});