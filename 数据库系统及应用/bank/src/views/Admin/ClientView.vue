<script setup>
    import Admin from '@/components/Admin.vue'

    // 客户管理:提供客户所有信息的增、删、改、查功能;如果客户存在着关联账户或者贷
    //      款记录,则不允许删除;
</script>

<script>
    import queryServer from '@/utils/queryServer'

    // create table client
    // (
    //    clientID             char(20) not null,
    //    clientName           char(10) not null,
    //    clientPhone          numeric(8,0) not null,
    //    clientAddress        char(50),
    //    clientLinkmanName    char(10) not null,
    //    clientLinkmanPhone   numeric(8,0) not null,
    //    clientLinkmanEmail   char(40) not null,
    //    clientLinkmanRelation char(10) not null,
    //    primary key (clientID)
    // );

    queryServer("insert into client values ('1', '张三', '103', '北京市', '李四', '104', 'li4@g.com', '朋友')")
    queryServer("insert into client values ('2', '李四', '104', '杭州市', '张六', '105', 'zhang6@com', '同事')")
    queryServer("insert into client values ('3', '王五', '105', '上海市', '赵七', '106', 'zhao7@com', '家人')")
    queryServer("insert into client values ('4', '赵八', '106', '广州市', '陈九', '107', 'chen9@com', '同学')")

    export default {
        data() {
            return {
                clientData: [],
                updateClientID: -1,
                inputClientID: '',
                inputClientName: '',
                inputClientPhone: '',
                inputClientAddress: '',
                inputClientLinkmanName: '',
                inputClientLinkmanPhone: '',
                inputClientLinkmanEmail: '',
                inputClientLinkmanRelation: ''
            }
        },
        methods: {
            queryAllData() {
                this.clientData = queryServer('select * from client')
                console.log(this.clientData)
            },
            deleteClient(index) {
                let clientID = this.clientData[index].clientID
                let allClientAccount = queryServer(`select * from clientAccount where clientID = '${clientID}'`)
                let allClientLoan = queryServer(`select * from clientLoan where clientID = '${clientID}'`)
                if (allClientAccount.length > 0) {
                    alert('该客户存在关联账户,不允许删除')
                } else if (allClientLoan.length > 0) {
                    alert('该客户存在关联贷款记录,不允许删除')
                } else {
                    queryServer(`delete from client where clientID = '${clientID}'`)
                    this.queryAllData()
                }
            },
            insertClient() {
                let inputClientName = this.inputClientName
                if (inputClientName.indexOf("'") != -1) {
                    inputClientName = inputClientName.replace(/'/g, "''")
                    console.log(inputClientName)
                }
                let command = `insert into client values ('${this.inputClientID}', '${inputClientName}', '${this.inputClientPhone}', '${this.inputClientAddress}', '${this.inputClientLinkmanName}', '${this.inputClientLinkmanPhone}', '${this.inputClientLinkmanEmail}', '${this.inputClientLinkmanRelation}');`
                queryServer(command)
                this.updateClientID = -1
                this.queryAllData()
            },
            moveToInput(index) {
                this.updateClientID = index
                this.inputClientID = this.clientData[index].clientID
                this.inputClientName = this.clientData[index].clientName
                this.inputClientPhone = this.clientData[index].clientPhone
                this.inputClientAddress = this.clientData[index].clientAddress
                this.inputClientLinkmanName = this.clientData[index].clientLinkmanName
                this.inputClientLinkmanPhone = this.clientData[index].clientLinkmanPhone
                this.inputClientLinkmanEmail = this.clientData[index].clientLinkmanEmail
                this.inputClientLinkmanRelation = this.clientData[index].clientLinkmanRelation
                console.log(this.inputClientID)
            },
            updateClient() {
                let inputClientName = this.inputClientName
                if (inputClientName.indexOf("'") != -1) {
                    inputClientName = inputClientName.replace(/'/g, "''")
                    console.log(inputClientName)
                }
                let command = `update client set clientName = '${inputClientName}', clientPhone = '${this.inputClientPhone}', clientAddress = '${this.inputClientAddress}', clientLinkmanName = '${this.inputClientLinkmanName}', clientLinkmanPhone = '${this.inputClientLinkmanPhone}', clientLinkmanEmail = '${this.inputClientLinkmanEmail}', clientLinkmanRelation = '${this.inputClientLinkmanRelation}' where clientID = '${this.inputClientID}';`
                queryServer(command)
                this.queryAllData()
            },
            queryInputData() {
                // 模糊查询
                let command = `select * from client where clientID like '%${this.inputClientID}%' and clientName like '%${this.inputClientName}%' and clientPhone like '%${this.inputClientPhone}%' and clientAddress like '%${this.inputClientAddress}%' and clientLinkmanName like '%${this.inputClientLinkmanName}%' and clientLinkmanPhone like '%${this.inputClientLinkmanPhone}%' and clientLinkmanEmail like '%${this.inputClientLinkmanEmail}%' and clientLinkmanRelation like '%${this.inputClientLinkmanRelation}%';`
                this.clientData = queryServer(command)
            }
        }
    }
</script>

<template>
    <Admin highlight="0" />

    <div>
        <button @click="queryInputData" class="queryButton">查询数据</button>
    </div>

    <div class="clientTable">
        <table>
            <thead>
                <tr>
                    <th>客户编号</th>
                    <th>客户姓名</th>
                    <th>客户电话</th>
                    <th>客户地址</th>
                    <th>客户联系人姓名</th>
                    <th>客户联系人电话</th>
                    <th>客户联系人邮箱</th>
                    <th>客户联系人关系</th>
                    <th>插入删除</th>
                    <th>修改</th>
                </tr>
            </thead>
            <tbody>
                <tr class="insertRow">
                    <td>
                        <input v-model="inputClientID" class="insertInput" type="text" />
                    </td>
                    <td>
                        <input v-model="inputClientName" class="insertInput" type="text" />
                    </td>
                    <td>
                        <input v-model="inputClientPhone" class="insertInput" type="text" />
                    </td>
                    <td>
                        <input v-model="inputClientAddress" class="insertInput" type="text" />
                    </td>
                    <td>
                        <input v-model="inputClientLinkmanName" class="insertInput" type="text" />
                    </td>
                    <td>
                        <input v-model="inputClientLinkmanPhone" class="insertInput" type="text" />
                    </td>
                    <td>
                        <input v-model="inputClientLinkmanEmail" class="insertInput" type="text" />
                    </td>
                    <td>
                        <input v-model="inputClientLinkmanRelation" class="insertInput" type="text" />
                    </td>
                    <td>
                        <button @click="insertClient">插入</button>
                    </td>
                    <td>
                        <button @click="updateClient">更新</button>
                    </td>
                </tr>

                <tr class="clientRow" v-for="(client, index) in clientData">
                    <td>{{ client.clientID }}</td>
                    <td>{{ client.clientName }}</td>
                    <td>{{ client.clientPhone }}</td>
                    <td>{{ client.clientAddress }}</td>
                    <td>{{ client.clientLinkmanName }}</td>
                    <td>{{ client.clientLinkmanPhone }}</td>
                    <td>{{ client.clientLinkmanEmail }}</td>
                    <td>{{ client.clientLinkmanRelation }}</td>
                    <td>
                        <button @click="deleteClient(index)">删除</button>
                    </td>
                    <td>
                        <button @click="moveToInput(index)">修改</button>
                    </td>
                </tr>
            </tbody>
        </table>
    </div>
</template>

<style src="@/assets/queryTableView.css"></style>
