<script lang="ts">
    // 银行管理系统 用户界面 center
    // 登录服务端模块
    //     服务端 IP     default show: 127.0.0.1
    //     服务端 端口    default show: 4000
    //     数据库 用户名  default show: root
    //     数据库 密码    default show: x
    //     数据库 名称    default show: bank
    //     登录按钮 : 跳转到管理界面

    let serverIP: string = '127.0.0.1'
    let serverPort: number = 4000
    let username: string = 'root'
    let password: string = 'x'
    let databaseName: string = 'bank'

    // 登录时发送到 serverIP: serverPort/mysql 的请求
    // 发送的数据格式为 JSON
    //     {
    //         username: '',
    //         password: '',
    //         databaseName: ''
    //     }

    export default {
        data() {
            return {
                serverIP: serverIP,
                serverPort: serverPort,
                username: username,
                password: password,
                databaseName: databaseName
            }
        },
        methods: {
            // 登录成功时返回的数据格式为 JSON
            // application/x-www-form-urlencoded
            login() {
                // 发送请求
                const request = new XMLHttpRequest()
                request.open('POST', `http://${serverIP}:${serverPort}/mysql`, false)
                request.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded')
                request.send(`username=${username}&password=${password}&databaseName=${databaseName}`)

                // 接收响应，200 代表成功
                if (request.status === 200) {
                    // 解析响应数据
                    const data = JSON.parse(request.responseText)
                    // 跳转到管理界面
                    window.location.href = './admin/client'
                } else {
                    console.log('login fail')
                }
            }
        }
    }
</script>

<template>
    <div class="LoginMain">
        <div class="title">
            <h1>银行管理系统</h1>
        </div>
        <!--<h2>用户界面</h2>-->
        <div class="login">
            <div class="login-input">
                <label>服务端 IP</label>
                <input type="text" v-model="serverIP" />
            </div>
            <div class="login-input">
                <label>服务端 端口</label>
                <input type="text" v-model="serverPort" />
            </div>
            <div class="login-input">
                <label>数据库 用户名</label>
                <input type="text" v-model="username" />
            </div>
            <div class="login-input">
                <label>数据库 密码</label>
                <input type="text" v-model="password" />
            </div>
            <div class="login-input">
                <label>数据库 名称</label>
                <input type="text" v-model="databaseName" />
            </div>
            <div class="login-button">
                <button @click="login">登录</button>
            </div>
        </div>
    </div>
</template>

<style>
    .login {
        width: 400px;
        margin: 0 auto;
        padding: 20px;
        border: 1px solid #ccc;
    }
    .login-input {
        margin-bottom: 10px;
    }
    .login-button {
        text-align: center;
    }
    label {
        display: inline-block;
        width: 150px;
    }
    input {
        display: inline-block;
        width: 200px;
    }
    .login-button button {
        width: 200px;
    }

    .title {
        text-align: center;
    }
</style>
