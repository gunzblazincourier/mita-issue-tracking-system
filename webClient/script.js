let issues = [];
let isEditingIssue = false;
let editingIssueId = null;

let users = [];
let isEditingUser = false;
let editingUserId = null;

let comment = [];

function handleKeyPress(event) {
  if (event.key === "Enter") {
    render();
  }
}

function render() {
    const list = document.getElementById("issues-list");
    list.innerHTML = "";

    const list2 = document.getElementById("users-list");
    list2.innerHTML = "";

    if (!issues.length) {
        list.innerHTML = '<p class="empty">No issues yet.</p>';
    }

    if (!users.length) {
        list2.innerHTML = '<p class="empty">No users.</p>';
    }

    issues.slice().reverse().forEach((issue) => {
        // Whenever render() is called, all issues displayed if search toolbar empty
        // Else issues search and displayed
        let search_id = document.getElementById("search").value;
        let search_id_user = document.getElementById("search3").value;
        let userIndex = 0;
        if (search_id_user != "") {
            userIndex = users.findIndex(user => user.id2 == search_id_user);
        }

        if ((search_id === "" || (search_id !== "" && issue.id == search_id)) &&
            (search_id_user === "" || (search_id_user !== "" && userIndex != -1 && search_id_user == issue.user_id))) {
            
            const wrap = document.createElement("div");
            wrap.className = "issue";
            wrap.innerHTML = `
                <div class="issue-main">
                    <h3 class="issue-title">${issue.title}</h3>
                    <p class="meta">#${issue.id} • <strong>${issue.priority}</strong></p>
                    <p class="meta"><b>${issue.status}</b> | <b><i>${issue.label}</i></b> </p>
                    <p class="issue-desc">${issue.description}</p>
                    <p class="meta">User <b>${issue.user_id}</b> • <u>${issue.user_name}</u></p>
                    <button class="btn edit-btn" data-id="${issue.id}">Edit</button>
                    <button class="btn delete-btn" data-id=${issue.id}">Delete</button>
                    <input type="number" class="assign" id="assign" placeholder="Assign user by ID" data-id="${issue.id}" />
                    <button class="btn unassign-btn" data-id=${issue.id}">Unassign User</button>
                    <div class="comments-section">
                        <h4>Comments:</h4>
                        <div class="comments-list" data-id=${issue.id}">
                            ${issue.comments.map((comment, index)=> `
                                <div class="comment">
                                    <p>${comment.text}</p>
                                    <button class="btn edit-comment-btn" data-issue-id="${issue.id}" data-comment-index=${index}>Edit</button>
                                    <button class="btn delete-comment-btn" data-issue-id="${issue.id}" data-comment-index=${index}>Delete</button>
                                </div>
                            `).join('')}
                        </div>
                    </div>
                    <textarea class="comment-input" placeholder="Add a comment..." data-issue-id="${issue.id}"></textarea>
                    <button class="btn add-comment-btn" data-issue-id="${issue.id}">Add Comment</button>
                </div>
            `;
            list.appendChild(wrap);
        }
    });

    users.slice().reverse().forEach((user) => {
        let search_id2 = document.getElementById("search2").value;
        if (search_id2 === "" || (search_id2 !== "" && user.id2 == search_id2)) {
            const wrap2 = document.createElement("div");
            wrap2.className = "issue";
            wrap2.innerHTML = `
                <div class="issue-main">
                    <p class="meta">#${user.id2}</p>
                    <h3 class="issue-title">${user.name}</h3>
                    <button class="btn edit-btn2" data-id="${user.id2}">Edit</button>
                    <button class="btn delete-btn2" data-id=${user.id2}">Delete</button>
                </div>
            `;
            list2.appendChild(wrap2);
        }
    });


    // Add event listeners for adding comments
    document.querySelectorAll(".add-comment-btn").forEach(button => {
        button.addEventListener("click", (e) => {
            const issueId = e.target.getAttribute("data-issue-id");
            const commentText = document.querySelector(`textarea[data-issue-id='${issueId}']`).value;
            if (commentText) {
                addComment(issueId, commentText);
            }
        });
    });

    document.querySelectorAll('.edit-comment-btn').forEach(button => {
        button.addEventListener('click', (e) => {
            const issueId = e.target.getAttribute('data-issue-id');
            const commentIndex = parseInt(e.target.getAttribute('data-comment-index'), 10);  // Get comment index
            editComment(issueId, commentIndex);
        });
    });

    document.querySelectorAll('.delete-comment-btn').forEach(button => {
        button.addEventListener('click', (e) => {
            const issueId = e.target.getAttribute('data-issue-id');
            const commentIndex = parseInt(e.target.getAttribute('data-comment-index'), 10);  // Get comment index
            deleteComment(issueId, commentIndex);
        });
    });

    // Add event listeners for edit button
    document.querySelectorAll('.edit-btn').forEach(button => {
        button.addEventListener('click', (e) => {
            const issueId = e.target.getAttribute('data-id');
            editIssue(issueId);
        });
    });

        // Add event listeners for delete button
    document.querySelectorAll('.delete-btn').forEach(button => {
        button.addEventListener('click', (e) => {
            const issueId = e.target.getAttribute('data-id');
            deleteIssue(issueId);
        });
    });

    document.querySelectorAll('.edit-btn2').forEach(button => {
        button.addEventListener('click', (e) => {
            const userId = e.target.getAttribute('data-id');
            editUser(userId);
        });
    });

        // Add event listeners for delete button
    document.querySelectorAll('.delete-btn2').forEach(button => {
        button.addEventListener('click', (e) => {
            const userId = e.target.getAttribute('data-id');
            deleteUser(userId);
        });
    });

    document.querySelectorAll('.assign').forEach(input => {
        input.addEventListener('keypress', (e) => {
            const issueId = e.target.getAttribute('data-id');
            const userId = e.target.value;
            assignUserToIssue(issueId, userId);
        });
    });

    document.querySelectorAll('.unassign-btn').forEach(button => {
        button.addEventListener('click', (e) => {
            const issueId = e.target.getAttribute('data-id');
            removeUserFromIssue(issueId);
        });
    });
}

search = document.getElementById("search");
search.addEventListener("keypress", handleKeyPress);

search2 = document.getElementById("search2");
search2.addEventListener("keypress", handleKeyPress);

search3 = document.getElementById("search3");
search3.addEventListener("keypress", handleKeyPress);

function init() {
    var counter = 0;
    const createBtn = document.getElementById("create-btn");
    const formWrap = document.getElementById("create-form");
    const form = document.getElementById("issue-form");
    const cancel = document.getElementById("cancel-create");

    var counter2 = 0;
    const createBtn2 = document.getElementById("create-btn2");
    const formWrap2 = document.getElementById("create-form2");
    const form2 = document.getElementById("user-form");
    const cancel2 = document.getElementById("cancel-create2");



    // open form
    createBtn.addEventListener("click", () => {
        formWrap.classList.toggle("visible");
        formWrap.setAttribute(
            "aria-hidden",
            String(!formWrap.classList.contains("visible"))
        );

        // Reset form if we're not editing
        if (!isEditingIssue) {
            form.reset();
            editingIssueId = null;
        }
    });

    createBtn2.addEventListener("click", () => {
        formWrap2.classList.toggle("visible");
        formWrap2.setAttribute(
            "aria-hidden",
            String(!formWrap2.classList.contains("visible"))
        );

        // Reset form if we're not editing
        if (!isEditingUser) {
            form2.reset();
            editingUserId = null;
        }
    });

    // cancel form
    cancel.addEventListener("click", () => {
        form.reset();
        formWrap.classList.remove("visible");
        formWrap.setAttribute("aria-hidden", "true");
        isEditingIssue = false;
    });

    cancel2.addEventListener("click", () => {
        form2.reset();
        formWrap2.classList.remove("visible");
        formWrap2.setAttribute("aria-hidden", "true");
        isEditingUser = false;
    });

    // submit form
    form.addEventListener("submit", (e) => {
        e.preventDefault();
        let id = 0;
        if (isEditingIssue && editingIssueId != null) {
            id = counter = counter;
        } else {
            id = counter = counter + 1;
        }
        const title = document.getElementById("title").value;
        const description = document.getElementById("description").value;
        const priority = document.getElementById("priority").value;
        const status = document.getElementById("status").value;
        const label = document.getElementById("label").value;
        if (isEditingIssue && editingIssueId != null) {
            // Edit existing issue
            console.log("Editing");
            fetch(`http://localhost:9100/issues/${editingIssueId}?title=${encodeURIComponent(title)}&description=${encodeURIComponent(description)}&priority=${encodeURIComponent(priority)}&status=${encodeURIComponent(status)}`,
                {
                    method: "PUT",
                }
            )
            .then(Response => Response.text())
            .then(data => {
                const issueIndex = issues.findIndex(issue => issue.id == editingIssueId);
                if (issueIndex != -1) {
                    issues[issueIndex] = {
                        id: editingIssueId,
                        title,
                        description,
                        priority,
                        status,
                        label,
                        user_id: issues[issueIndex].user_id,
                        user_name: issues[issueIndex].user_name,
                        comments: issues[issueIndex].comments
                    };
                    console.log("Issue updated");
                }
                render();
            })
            .catch(error => console.error("Error updating issue: ", error));

        } else {
            // Add new issue
            const newIssue = {
                id,
                title,
                description,
                priority,
                status,
                label,
                user_id: 0,
                user_name: "",
                comments: []
            };

            fetch(`http://localhost:9100/issues?id=${counter}&title=${title}&description=${description}`, 
                {
                    method: "POST",
                }
            )
            .then(Response => Response.text())
            .then(data => {
                console.log("Sent to server");
            })
            .catch(error => console.error("Error creating issue: ", error));
            issues.push(newIssue);
            console.log("New issue added");
        }



        // Reset form and state
        form.reset();
        formWrap.classList.remove("visible");
        formWrap.setAttribute("aria-hidden", "true");
        isEditingIssue = false;

        render();
    });

    form2.addEventListener("submit", (e) => {
        e.preventDefault();
        let id2 = 0;
        if (isEditingUser && editingUserId != null) {
            id2 = counter2 = counter2;
        } else {
            id2 = counter2 = counter2 + 1;
        }
        const name = document.getElementById("name").value;
        if (isEditingUser && editingUserId != null) {
            // Edit existing user
            console.log("Editing");
            fetch(`http://localhost:9120/users/${editingUserId}?name=${encodeURIComponent(name)}`,
                {
                    method: "PUT",
                }
            )
            .then(Response => Response.text())
            .then(data => {
                const userIndex = users.findIndex(user => user.id2 == editingUserId);
                if (userIndex != -1) {
                    users[userIndex] = {
                        id2: editingUserId,
                        name,
                    };
                    console.log("User updated");
                }
                render();
            })
            .catch(error => console.error("Error updating user: ", error));

        } else {
            // Add new user
            const newUser = {
                id2,
                name,
            };

            fetch(`http://localhost:9120/users?id=${counter2}&name=${encodeURIComponent(name)}`, 
                {
                    method: "POST",
                }
            )
            .then(Response => Response.text())
            .then(data => {
                console.log("Sent to server");
            })
            .catch(error => console.error("Error creating user: ", error));
            users.push(newUser);
            console.log("New user added");
        }



        // Reset form and state
        form.reset();
        formWrap.classList.remove("visible");
        formWrap.setAttribute("aria-hidden", "true");
        isEditingIssue = false;

        form2.reset();
        formWrap2.classList.remove("visible");
        formWrap2.setAttribute("aria-hidden", "true");
        isEditingUser = false;

        render();
    });

    render();
}

// Function to populate the form for editing
function editIssue(issueId) {
    const issue = issues.find(issue => issue.id == issueId);
    if (issue) {
        const form = document.getElementById("issue-form");
        document.getElementById("title").value = issue.title;
        document.getElementById("description").value = issue.description;
        document.getElementById("priority").value = issue.priority;
        document.getElementById("status").value = issue.status;
        document.getElementById("label").value = issue.label;

        // Set the form state to editing
        isEditingIssue = true;
        editingIssueId = issueId;

        const formWrap = document.getElementById("create-form");
        formWrap.classList.add("visible");
        formWrap.setAttribute("aria-hidden", "false");
    }else{
        console.log(`No issue ${issueId} found`);
    }
}

function editUser(userId) {
    const user = users.find(user => user.id2 == userId);
    if (user) {
        const form2 = document.getElementById("user-form");
        document.getElementById("name").value = user.name;

        // Set the form state to editing
        isEditingUser = true;
        editingUserId = userId;

        const formWrap2 = document.getElementById("create-form2");
        formWrap2.classList.add("visible");
        formWrap2.setAttribute("aria-hidden", "false");
    }else{
        console.log(`No user ${userId} found`);
    }
}

function deleteIssue(issueId) {
        // Trim any extra spaces or quotes from the issueId
    const cleanedIssueId = issueId.trim().replace(/['"]+/g, '');
    console.log("Deleting issue with ID:", cleanedIssueId);

    // Send DELETE request to server
    fetch(`http://localhost:9100/issues/${encodeURIComponent(cleanedIssueId)}`,{
        method: "DELETE",
    })
    .then(response => {
        if (response.ok) {
            console.log("Issue deleted on server");
            const issueIndex = issues.findIndex(issue => issue.id == cleanedIssueId);
            if (issueIndex != -1) {
                issues.splice(issueIndex, 1);
            }
            render(); // Re-render the issue list
        } else {
            console.error("Error deleting issue on server:", response.statusText);
        }
    })
    .catch(error => {
        console.error("Error with DELETE request:", error);
    });
}

function deleteUser(userId) {
        // Trim any extra spaces or quotes from the issueId
    const cleanedUserId = userId.trim().replace(/['"]+/g, '');
    console.log("Deleting user with ID:", cleanedUserId);

    // Send DELETE request to server
    fetch(`http://localhost:9120/users/${encodeURIComponent(cleanedUserId)}`,{
        method: "DELETE",
    })
    .then(response => {
        if (response.ok) {
            console.log("User deleted on server");
            const userIndex = users.findIndex(user => user.id2 == cleanedUserId);
            if (userIndex != -1) {
                users.splice(userIndex, 1);
            }
            issues.slice().reverse().forEach((issue) => {
                if (issue.user_id == cleanedUserId) {
                    removeUserFromIssue(issue.id);
                }
            });
            render();
        } else {
            console.error("Error deleting user on server:", response.statusText);
        }
    })
    .catch(error => {
        console.error("Error with DELETE request:", error);
    });
}

function assignUserToIssue(issueId, userId) {
    console.log("Assigning user to ID:", issueId);
        // Trim any extra spaces or quotes from the issueId
    const cleanedIssueId = issueId.trim().replace(/['"]+/g, '');
    const cleanedUserId = userId.trim().replace(/['"]+/g, '');

    // Send DELETE request to server
    fetch(`http://localhost:9100/issues/${encodeURIComponent(cleanedIssueId)}/users/assign?userId=${encodeURIComponent(cleanedUserId)}`,
        {
            method: "PUT",
        }
    )
    .then(Response => Response.text())
    .then(data => {
        const issueIndex = issues.findIndex(issue => issue.id == cleanedIssueId);
        const userIndex = users.findIndex(user => user.id2 == cleanedUserId);
        if (issueIndex != -1) {
            issues[issueIndex] = {
                id: cleanedIssueId,
                title: issues[issueIndex].title,
                description: issues[issueIndex].description,
                priority: issues[issueIndex].priority,
                status: issues[issueIndex].status,
                label: issues[issueIndex].label,
                user_id: cleanedUserId,
                user_name: users[userIndex].name,
                comments: issues[issueIndex].comments
            };
            console.log("User assigned!");
        }
        render();
    })
    .catch(error => console.error("Error assigning user: ", error));
}

function removeUserFromIssue(issueId) {
    console.log("Removing user from issue:", issueId);
        // Trim any extra spaces or quotes from the issueId
    const cleanedIssueId = issueId.trim().replace(/['"]+/g, '');

    // Send DELETE request to server
    fetch(`http://localhost:9100/issues/${encodeURIComponent(cleanedIssueId)}/users/unassign`,
        {
            method: "PUT",
        }
    )
    .then(Response => Response.text())
    .then(data => {
        const issueIndex = issues.findIndex(issue => issue.id == cleanedIssueId);
        if (issueIndex != -1) {
            issues[issueIndex] = {
                id: cleanedIssueId,
                title: issues[issueIndex].title,
                description: issues[issueIndex].description,
                priority: issues[issueIndex].priority,
                status: issues[issueIndex].status,
                label: issues[issueIndex].label,
                user_id: 0,
                user_name: "",
                comments: issues[issueIndex].comments
            };
            console.log("User unassigned!");
        }
        render();
    })
    .catch(error => console.error("Error unassigning user: ", error));
}

// Function to add a comment to an issue
function addComment(issueId, commentText) {
    const issue = issues.find(issue => issue.id == issueId);
    if (issue) {
        const newComment = {
            text: commentText,  // No ID or user info
        };
        issue.comments = issue.comments || [];
        issue.comments.push(newComment);

        // Update the server with the new comment
        fetch(`http://localhost:9100/issues/${issueId}/comments?content=${encodeURIComponent(commentText)}`, {
            method: 'POST',
        })
        .then(response => response.text())
        .then(data => {
            console.log("Comment added");
            render();
        })
        .catch(error => console.error("Error adding comment:", error));
    }
}

function editComment(issueId, commentIndex) {
    const issue = issues.find(issue => issue.id == issueId);
    if (issue) {
        const comment = issue.comments[commentIndex];  // Use the index directly
        if (comment) {
            const newCommentText = prompt("Edit comment:", comment.text);
            if (newCommentText !== null) {
                // Update the comment text
                comment.text = newCommentText;

                // Update on the server (if necessary)
                fetch(`http://localhost:9100/issues/${issueId}/comments/${commentIndex + 1}?content=${encodeURIComponent(newCommentText)}`, {
                    method: 'PUT',
                })
                .then(response => response.text())
                .then(data => {
                    console.log("Comment updated");
                    render(); // Re-render the list to reflect the changes
                })
                .catch(error => console.error("Error updating comment:", error));
            }
        } else {
            console.log("Comment not found.");
        }
    } else {
        console.log("Issue not found.");
    }
}


function deleteComment(issueId, commentIndex) {
    const issue = issues.find(issue => issue.id == issueId);
    if (issue) {
        const comment = issue.comments[commentIndex];  // Access by index
        if (comment) {
            // Remove comment from the issue's comment array
            issue.comments.splice(commentIndex, 1);

            // Delete from server (if necessary)
            fetch(`http://localhost:9100/issues/${issueId}/comments/${commentIndex + 1}`, {
                method: 'DELETE',
            })
            .then(response => response.text())
            .then(data => {
                console.log("Comment deleted");
                render(); // Re-render the list
            })
            .catch(error => console.error("Error deleting comment:", error));
        }
    }
}

if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", init);
} else {
    init();
}