let serverIP = '127.0.0.1'
let serverPort = 4000

function queryServer(command) {
    let request = new XMLHttpRequest()
    request.open('POST', `http://${serverIP}:${serverPort}/mysql`, false)
    request.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded')
    request.send(`command=${command}`)

    return JSON.parse(request.responseText)
}


export default queryServer
