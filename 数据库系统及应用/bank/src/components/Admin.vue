<script>
    // 数据库管理界面
    // 客户管理:提供客户所有信息的增、删、改、查功能;如果客户存在着关联账户或者贷
    //      款记录,则不允许删除;
    // 账户管理:提供账户开户、销户、修改、查询功能,包括储蓄账户和支票账户;账户号
    //      不允许修改;
    // 贷款管理:提供贷款信息的增、删、查功能,提供贷款发放功能;贷款信息一旦添加成
    //      功后不允许修改;要求能查询每笔贷款的当前状态(未开始发放、发放中、已全部发
    //      放);处于发放中状态的贷款记录不允许删除;
    // 业务统计:按业务分类(储蓄、贷款)和时间(月、季、年)统计各个支行的业务总金
    //      额和用户数,统计的结果以表格形式展示。

    // highlight
    // 0: client
    // 1: account
    // 2: loan
    // 3: statistics
    export default {
        props: ['highlight']
    }
</script>

<template>
    <div class="AdminMain">
        <div class="title">
            <h1>银行管理系统</h1>
        </div>
        <div class="admin-menu">
            <ul>
                <li>
                    <router-link to="/admin/client" :class="{ highlight: highlight === '0' }">客户管理</router-link>
                </li>
                <li>
                    <router-link to="/admin/account" :class="{ highlight: highlight === '1' }">账户管理</router-link>
                </li>
                <li>
                    <router-link to="/admin/loan" :class="{ highlight: highlight === '2' }">贷款管理</router-link>
                </li>
                <li>
                    <router-link to="/admin/statistics" :class="{ highlight: highlight === '3' }">业务统计</router-link>
                </li>
            </ul>
        </div>
        <router-view></router-view>
    </div>
</template>

<style>
    .AdminMain {
        width: 100%;
        height: 100%;
        display: flex;
        flex-direction: column;
        justify-content: center;
        align-items: center;
    }
    .title {
        width: 100%;
        height: 50px;
        display: flex;
        justify-content: center;
        align-items: center;
    }
    .title h1 {
        font-size: 30px;
        font-weight: bold;
    }
    .admin-menu {
        width: 100%;
        height: 50px;
        display: flex;
        justify-content: center;
        align-items: center;
        background-color: #f5f5f5;
    }
    .admin-menu ul {
        width: 100%;
        height: 100%;
        display: flex;
        justify-content: space-around;
        align-items: center;
    }
    .admin-menu li {
        width: 100%;
        height: 100%;
        display: flex;
        justify-content: center;
        align-items: center;
    }
    .admin-menu li a {
        font-size: 20px;
        font-weight: bold;
    }
    .highlight {
        color: #ff0000;
    }
</style>
