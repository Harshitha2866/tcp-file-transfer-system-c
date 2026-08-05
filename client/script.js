const API = "http://localhost:3000";

async function uploadFile() {
    const fileInput = document.getElementById("fileInput");

    if (fileInput.files.length === 0) {
        alert("Please select a file.");
        return;
    }

    const formData = new FormData();
    formData.append("file", fileInput.files[0]);

    const response = await fetch(API + "/upload", {
        method: "POST",
        body: formData
    });

    const result = await response.json();

    document.getElementById("status").innerText = result.message;

    loadFiles();
}

async function loadFiles() {

    const response = await fetch(API + "/files");
    const files = await response.json();

    document.getElementById("totalFiles").innerText = files.length;

    const fileList = document.getElementById("fileList");

    fileList.innerHTML = "";

    files.forEach(file => {

        const li = document.createElement("li");

        let icon = '<i class="fa-solid fa-file"></i>';

        if (file.endsWith(".png") || file.endsWith(".jpg") || file.endsWith(".jpeg")) {
            icon = '<i class="fa-solid fa-file-image"></i>';
        }
        else if (file.endsWith(".pdf")) {
            icon = '<i class="fa-solid fa-file-pdf"></i>';
        }
        else if (file.endsWith(".txt")) {
            icon = '<i class="fa-solid fa-file-lines"></i>';
        }
        else if (file.endsWith(".zip") || file.endsWith(".rar")) {
            icon = '<i class="fa-solid fa-file-zipper"></i>';
        }
        else if (file.endsWith(".mp4")) {
            icon = '<i class="fa-solid fa-file-video"></i>';
        }
        else if (file.endsWith(".mp3")) {
            icon = '<i class="fa-solid fa-file-audio"></i>';
        }

        li.innerHTML = `
            <span>${icon} ${file}</span>

            <div class="actions">
                <button onclick="downloadFile('${file}')">
                    ⬇ Download
                </button>

                <button class="delete-btn"
                        onclick="deleteFile('${file}')">
                    🗑 Delete
                </button>
            </div>
        `;

        fileList.appendChild(li);

    });

}

function downloadFile(filename) {

    window.open(API + "/download/" + filename);

}

async function deleteFile(filename) {

    const confirmDelete = confirm(
        "Delete " + filename + " ?"
    );

    if (!confirmDelete) return;

    const response = await fetch(
        API + "/delete/" + filename,
        {
            method: "DELETE"
        }
    );

    const result = await response.json();

    alert(result.message);

    loadFiles();

}

loadFiles();

function searchFiles() {

    const input =
        document.getElementById("search").value.toLowerCase();

    const files =
        document.querySelectorAll("#fileList li");

    files.forEach(file => {

        const text =
            file.innerText.toLowerCase();

        if (text.includes(input))
            file.style.display = "flex";
        else
            file.style.display = "none";

    });

}