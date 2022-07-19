<script setup>
    import Admin from '@/components/Admin.vue'

    //  账户管理:提供账户开户、销户、修改、查询功能,包括储蓄账户和支票账户;账户号
    //      不允许修改;
</script>

<script>
    import queryServer from '@/utils/queryServer'

    // create table bankAccount
    // (
    //    accountID            char(30) not null,
    //    accountRemain        float(8,2) not null,
    //    accountOpenDate      date not null,
    //    primary key (accountID)
    // );

    // create table depositAccount
    // (
    //    accountID            char(30) not null,
    //    subbranchName        char(20) not null,
    //    accountRemain        float(8,2) not null,
    //    accountOpenDate      date not null,
    //    moneyRate            float not null,
    //    currencyType         char(10) not null,
    //    primary key (accountID)
    // );

    // create table checkAccount
    // (
    //    accountID            char(30) not null,
    //    subbranchName        char(20) not null,
    //    accountRemain        float(8,2) not null,
    //    accountOpenDate      date not null,
    //    overdraft            float(8,2) not null,
    //    primary key (accountID)
    // );

    // create table depositClient
    // (
    //    accountID            char(30) not null,
    //    clientID             char(20) not null,
    //    primary key (accountID, clientID)
    // );

    // create table checkClient
    // (
    //    accountID            char(30) not null,
    //    clientID             char(20) not null,
    //    primary key (clientID, accountID)
    // );

    queryServer("insert into bankAccount values ('100', '10000', '2020-01-01')")
    queryServer("insert into bankAccount values ('102', '20000', '2020-02-01')")
    queryServer("insert into bankAccount values ('201', '30000', '2020-03-02')")
    queryServer("insert into bankAccount values ('202', '40000', '2020-04-03')")
    queryServer("insert into bankAccount values ('203', '50000', '2021-05-04')")

    queryServer("insert into depositAccount values ('100', '支行A', '10000', '2020-01-01', '0.05', 'CNY')")
    queryServer("insert into depositAccount values ('102', '支行B', '20000', '2020-02-01', '0.07', 'USD')")
    queryServer("insert into checkAccount values ('201', '支行A', '30000', '2020-03-02', '3000')")
    queryServer("insert into checkAccount values ('202', '支行C', '40000', '2020-04-03', '5000')")
    queryServer("insert into checkAccount values ('203', '支行B', '50000', '2021-05-04', '6000')")

    queryServer("insert into depositClient values ('100', '1')")
    queryServer("insert into depositClient values ('102', '3')")
    queryServer("insert into checkClient values ('201', '1')")
    queryServer("insert into checkClient values ('202', '2')")
    queryServer("insert into checkClient values ('202', '3')")
    queryServer("insert into checkClient values ('203', '1')")

    export default {
        data() {
            return {
                depositData: [],
                checkData: [],
                depositIndex: -1,
                checkIndex: -1,
                inputAccountID: '',
                inputAccountRemain: '',
                inputAccountOpenDate: '',
                inputSubbranchName: '',
                inputMoneyRate: '', // 储蓄账户
                inputCurrencyType: '', // 储蓄账户
                inputOverdraft: '', // 支票账户
                inputAllClientID: ''
            }
        },
        methods: {
            queryAllData() {
                // 获取 accountID 在 depositClient 中对应的所有 clientID，用逗号分隔，拼接到一个字符串中
                // 作为 depositAccount 的 allClientID 字段，返回
                let depositCommand =
                    'select * from depositAccount natural join (select accountID, group_concat(clientID) as allClientID from depositClient group by accountID) as temp'
                let checkCommand =
                    'select * from checkAccount natural join (select accountID, group_concat(clientID) as allClientID from checkClient group by accountID) as temp'
                this.depositData = queryServer(depositCommand)
                this.checkData = queryServer(checkCommand)
                console.log(this.depositData)
                console.log(this.checkData)
            },
            deleteDepositAccount(depositIndex) {
                let accountID = this.depositData[depositIndex].accountID

                // 删除 depositClient 中对应的记录
                queryServer(`delete from depositClient where accountID = '${accountID}'`)
                queryServer(`delete from depositAccount where accountID = '${accountID}'`)
                queryServer(`delete from bankAccount where accountID = '${accountID}'`)
                this.queryAllData()
            },
            deleteCheckAccount(checkIndex) {
                let accountID = this.checkData[checkIndex].accountID
                queryServer(`delete from checkClient where accountID = '${accountID}'`)
                queryServer(`delete from checkAccount where accountID = '${accountID}'`)
                queryServer(`delete from bankAccount where accountID = '${accountID}'`)
                this.queryAllData()
            },
            insertDepositAccount() {
                // 储蓄账户
                // for every clientID in inputAllClientID
                //   try to query
                // 一个用户在一个支行只能开一个储蓄账户
                // if found, alert "该客户已经存在于该账户中" and return

                let allClientIDList = this.inputAllClientID.split(',')
                let subbranchName = this.inputSubbranchName
                for (let clientID of allClientIDList) {
                    let command = `select * from depositAccount natural join depositClient where clientID = '${clientID}' and subbranchName = '${subbranchName}'`
                    let result = queryServer(command)
                    if (result.length > 0) {
                        alert('该客户已经存在于该账户中')
                        return
                    }
                }
                let command = `insert into bankAccount values ('${this.inputAccountID}', '${this.inputAccountRemain}', '${this.inputAccountOpenDate}')`
                queryServer(command)
                command = `insert into depositAccount values ('${this.inputAccountID}', '${this.inputSubbranchName}', '${this.inputAccountRemain}', '${this.inputAccountOpenDate}', '${this.inputMoneyRate}', '${this.inputCurrencyType}')`
                queryServer(command)

                for (let item of allClientIDList) {
                    let command = `insert into depositClient values ('${this.inputAccountID}', '${item}')`
                    queryServer(command)
                }

                this.depositIndex = -1
                this.queryAllData()
            },
            insertCheckAccount() {
                // 支票账户

                let allClientIDList = this.inputAllClientID.split(',')
                let subbranchName = this.inputSubbranchName
                for (let clientID of allClientIDList) {
                    let command = `select * from checkAccount natural join checkClient where clientID = '${clientID}' and subbranchName = '${subbranchName}'`
                    let result = queryServer(command)
                    if (result.length > 0) {
                        alert('该客户已经存在于该账户中')
                        return
                    }
                }
                let command = `insert into bankAccount values ('${this.inputAccountID}', '${this.inputAccountRemain}', '${this.inputAccountOpenDate}')`
                queryServer(command)
                command = `insert into checkAccount values ('${this.inputAccountID}', '${this.inputSubbranchName}', '${this.inputAccountRemain}', '${this.inputAccountOpenDate}', '${this.inputOverdraft}')`
                queryServer(command)

                for (let item of allClientIDList) {
                    let command = `insert into checkClient values ('${this.inputAccountID}', '${item}')`
                    queryServer(command)
                }

                this.checkIndex = -1
                this.queryAllData()
            },
            moveDepositToInput(depositIndex) {
                this.depositIndex = depositIndex
                this.inputAccountID = this.depositData[depositIndex].accountID
                this.inputAccountRemain = this.depositData[depositIndex].accountRemain
                this.inputAccountOpenDate = this.depositData[depositIndex].accountOpenDate
                this.inputSubbranchName = this.depositData[depositIndex].subbranchName
                this.inputAccountOpenDate = this.depositData[depositIndex].accountOpenDate
                this.inputMoneyRate = this.depositData[depositIndex].moneyRate
                this.inputCurrencyType = this.depositData[depositIndex].currencyType
                this.inputAllClientID = this.depositData[depositIndex].allClientID
            },
            moveCheckToInput(checkIndex) {
                this.checkIndex = checkIndex
                console.log(this.checkIndex)
                this.inputAccountID = this.checkData[checkIndex].accountID
                this.inputAccountRemain = this.checkData[checkIndex].accountRemain
                this.inputAccountOpenDate = this.checkData[checkIndex].accountOpenDate
                this.inputSubbranchName = this.checkData[checkIndex].subbranchName
                this.inputAccountOpenDate = this.checkData[checkIndex].accountOpenDate
                this.inputOverdraft = this.checkData[checkIndex].overdraft
                this.inputAllClientID = this.checkData[checkIndex].allClientID
            },
            updateDepositAccount() {
                if (this.inputAccountID !== this.depositData[this.depositIndex].accountID) {
                    alert('账户ID不能修改')
                    return
                }
                let command = `update depositAccount set subbranchName = '${this.inputSubbranchName}', accountRemain = '${this.inputAccountRemain}', accountOpenDate = '${this.inputAccountOpenDate}', moneyRate = '${this.inputMoneyRate}', currencyType = '${this.inputCurrencyType}' where accountID = '${this.inputAccountID}'`
                queryServer(command)
                command = `update bankAccount set accountRemain = '${this.inputAccountRemain}', accountOpenDate = '${this.inputAccountOpenDate}' where accountID = '${this.inputAccountID}'`
                queryServer(command)

                let prevAllClientIDList = this.depositData[this.depositIndex].allClientID.split(',')
                for (let item of prevAllClientIDList) {
                    let command = `delete from depositClient where accountID = '${this.inputAccountID}' and clientID = '${item}'`
                    queryServer(command)
                }
                let allClientIDList = this.inputAllClientID.split(',')
                for (let item of allClientIDList) {
                    let command = `insert into depositClient values ('${this.inputAccountID}', '${item}')`
                    queryServer(command)
                }

                this.queryAllData()
            },
            updateCheckAccount() {
                if (this.inputAccountID !== this.checkData[this.checkIndex].accountID) {
                    alert('账户ID不能修改')
                    return
                }
                let command = `update checkAccount set subbranchName = '${this.inputSubbranchName}', accountRemain = '${this.inputAccountRemain}', accountOpenDate = '${this.inputAccountOpenDate}', overdraft = '${this.inputOverdraft}' where accountID = '${this.inputAccountID}'`
                queryServer(command)
                command = `update bankAccount set accountRemain = '${this.inputAccountRemain}', accountOpenDate = '${this.inputAccountOpenDate}' where accountID = '${this.inputAccountID}'`
                queryServer(command)

                let prevAllClientIDList = this.checkData[this.checkIndex].allClientID.split(',')
                for (let item of prevAllClientIDList) {
                    let command = `delete from checkClient where accountID = '${this.inputAccountID}' and clientID = '${item}'`
                    queryServer(command)
                }
                let allClientIDList = this.inputAllClientID.split(',')
                for (let item of allClientIDList) {
                    let command = `insert into checkClient values ('${this.inputAccountID}', '${item}')`
                    queryServer(command)
                }

                this.queryAllData()
            },
            queryInputData() {
                // 模糊查询
                let depositCommand = `select * from (select * from depositAccount natural join (select accountID, group_concat(clientID) as allClientID from depositClient group by accountID) as temp) as temp2 where accountID like '%${this.inputAccountID}%' and subbranchName like '%${this.inputSubbranchName}%' and accountRemain like '%${this.inputAccountRemain}%' and accountOpenDate like '%${this.inputAccountOpenDate}%' and moneyRate like '%${this.inputMoneyRate}%' and currencyType like '%${this.inputCurrencyType}%' and allClientID like '%${this.inputAllClientID}%'`
                let checkCommand = `select * from (select * from checkAccount natural join (select accountID, group_concat(clientID) as allClientID from checkClient group by accountID) as temp) as temp2 where accountID like '%${this.inputAccountID}%' and subbranchName like '%${this.inputSubbranchName}%' and accountRemain like '%${this.inputAccountRemain}%' and accountOpenDate like '%${this.inputAccountOpenDate}%' and overdraft like '%${this.inputOverdraft}%' and allClientID like '%${this.inputAllClientID}%'`
                this.depositData = queryServer(depositCommand)
                this.checkData = queryServer(checkCommand)
                console.log(this.depositData)
                console.log(this.checkData)
            }
        }
    }
</script>

<template>
    <Admin highlight="1" />

    <div>
        <button @click="queryInputData" class="queryButton">查询数据</button>
    </div>

    <div class="depositTable">
        <div>
            <h2>储蓄账户</h2>
        </div>
        <table>
            <thead>
                <tr>
                    <th>账户ID</th>
                    <th>账户余额</th>
                    <th>账户开户日期</th>
                    <th>开户支行</th>
                    <th>储蓄账户利率</th>
                    <th>储蓄账户货币类型</th>
                    <th>关联客户</th>
                    <th>插入删除</th>
                    <th>修改</th>
                </tr>
            </thead>
            <tbody>
                <tr>
                    <td>
                        <input class="insertInput" v-model="inputAccountID" />
                    </td>
                    <td>
                        <input class="insertInput" v-model="inputAccountRemain" />
                    </td>
                    <td>
                        <input class="insertInput" v-model="inputAccountOpenDate" />
                    </td>
                    <td>
                        <input class="insertInput" v-model="inputSubbranchName" />
                    </td>
                    <td>
                        <input class="insertInput" v-model="inputMoneyRate" />
                    </td>
                    <td>
                        <input class="insertInput" v-model="inputCurrencyType" />
                    </td>
                    <td>
                        <input class="insertInput" v-model="inputAllClientID" />
                    </td>
                    <td>
                        <button @click="insertDepositAccount" class="insertButton">插入</button>
                    </td>
                    <td>
                        <button @click="updateDepositAccount" class="updateButton">更新</button>
                    </td>
                </tr>

                <tr class="accountRow" v-for="(account, depositIndex) in depositData">
                    <td>{{ account.accountID }}</td>
                    <td>{{ account.accountRemain }}</td>
                    <td>{{ account.accountOpenDate }}</td>
                    <td>{{ account.subbranchName }}</td>
                    <td>{{ account.moneyRate }}</td>
                    <td>{{ account.currencyType }}</td>
                    <td>{{ account.allClientID }}</td>

                    <td>
                        <button @click="deleteDepositAccount(depositIndex)">删除</button>
                    </td>
                    <td>
                        <button @click="moveDepositToInput(depositIndex)">修改</button>
                    </td>
                </tr>
            </tbody>
        </table>
    </div>

    <div class="checkTable">
        <div>
            <h2>支票账户</h2>
        </div>
        <table>
            <thead>
                <tr>
                    <th>账户ID</th>
                    <th>账户余额</th>
                    <th>账户开户日期</th>
                    <th>开户支行</th>
                    <th>支票账户可取款额度</th>
                    <th>关联客户</th>
                    <th>插入删除</th>
                    <th>修改</th>
                </tr>
            </thead>
            <tbody>
                <tr>
                    <td>
                        <input class="insertInput" v-model="inputAccountID" />
                    </td>
                    <td>
                        <input class="insertInput" v-model="inputAccountRemain" />
                    </td>
                    <td>
                        <input class="insertInput" v-model="inputAccountOpenDate" />
                    </td>
                    <td>
                        <input class="insertInput" v-model="inputSubbranchName" />
                    </td>
                    <td>
                        <input class="insertInput" v-model="inputOverdraft" />
                    </td>
                    <td>
                        <input class="insertInput" v-model="inputAllClientID" />
                    </td>
                    <td>
                        <button @click="insertCheckAccount" class="insertButton">插入</button>
                    </td>
                    <td>
                        <button @click="updateCheckAccount" class="updateButton">更新</button>
                    </td>
                </tr>

                <tr class="accountRow" v-for="(account, checkIndex) in checkData">
                    <td>{{ account.accountID }}</td>
                    <td>{{ account.accountRemain }}</td>
                    <td>{{ account.accountOpenDate }}</td>
                    <td>{{ account.subbranchName }}</td>
                    <td>{{ account.overdraft }}</td>
                    <td>{{ account.allClientID }}</td>

                    <td>
                        <button @click="deleteCheckAccount(checkIndex)">删除</button>
                    </td>
                    <td>
                        <button @click="moveCheckToInput(checkIndex)">修改</button>
                    </td>
                </tr>
            </tbody>
        </table>
    </div>
</template>

<style src="@/assets/queryTableView.css"></style>
